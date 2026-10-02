#include "input_task.h"

#include "button_state.hpp"
#include "cmsis_os2.h"
#include "encoder_id.hpp"
#include "encoder_rotation_direction.hpp"
#include "input_event_type.hpp"
#include "input_initparams.h"
#include "input_messages.h"
#include "mcp23017.hpp"
#include "mcp23017_gpio_map.hpp"
#include "mcp23017_gpio_to_button_map.hpp"
#include "mutable_enum_map.hpp"
#include "state_event_type.hpp"
#include "state_messages.h"
#include "system_init_event_flag.hpp"
#include "track_config.h"
#include "utils.h"

#define BUTTON_HOLD_THRESHOLD_TICKS pdMS_TO_TICKS(2000U)
#define BUTTON_DOUBLE_TAP_THRESHOLD_TICKS pdMS_TO_TICKS(200U)

struct ButtonEventContext {
  TickType_t last_pressed_ticks;  // 마지막으로 PRESSED 이벤트가 발생한 틱
  TickType_t next_wait_ticks;     // 검사를 위해 깨어나야 할 미래의 틱
  ButtonId id;
  ButtonState last_state;
};

struct TrackStopButtonContext {
  ButtonEventContext hold_event_context;
  ButtonEventContext double_tap_event_context;
};

struct InputTaskContext {
  MutableEnumMap<EncoderId, ButtonState> encoder_button_states{
      EnumEntry{EncoderId::A, ButtonState::RELEASED},
      EnumEntry{EncoderId::B, ButtonState::RELEASED},
      EnumEntry{EncoderId::C, ButtonState::RELEASED},
      EnumEntry{EncoderId::D, ButtonState::RELEASED},
  };
  std::array<TrackStopButtonContext, TRACK_COUNT> track_stop_button_contexts;
  TickType_t next_wait_ticks;
};

// TODO: InputTaskContext에 넣기
static osMessageQueueId_t input_event_queue;
static osMessageQueueId_t state_event_queue;

static void Run(void);
static TaskStatus HandleInputEvent(RtosMessage_InputEvent* input_event);
static TaskStatus HandleMcp23017IntEvent(RtosPayload_Mcp23017Event* intEvent);
static TaskStatus HandleMcp23017PortSnapshot(
    Mcp23017::Address address, Mcp23017::Port port,
    Mcp23017::PortInterruptSnapshot snapshot, TickType_t timestamp_ticks);
static TaskStatus TryHandleTrackStopButton(ButtonId button_id,
                                           ButtonState button_state,
                                           TickType_t timestamp_ticks);
static void DetectButtonDoubleTapEvent(ButtonEventContext& context,
                                       ButtonState state,
                                       TickType_t timestamp_ticks);
static void DetectButtonHoldEvent(ButtonEventContext& context,
                                  ButtonState state,
                                  TickType_t timestamp_ticks);
static TickType_t FindLowestNextWaitTicks(TickType_t timestamp_ticks);
static TaskStatus SendButtonPayload(ButtonId id, ButtonState state,
                                    TickType_t timestamp_ticks);
static void UpdateButtonContext(ButtonId id, ButtonState state,
                                TickType_t timestamp_ticks);
static TaskStatus HandleEncoderRotationEvent(
    RtosPayload_EncoderRotation* encoder_rotation_event);
static TaskStatus HandleAdcConversionEvent(
    RtosPayload_AdcConversion* adc_conversion_event);
static void HandleButtonHoldEvent();

static InputTaskContext input_task_context;

static constexpr std::array<ButtonId, TRACK_COUNT> stop_button_ids{
    ButtonId::TRACK_1_STOP,
};

static int IsValidInitParams(const InputInitParams* params) {
  return (params != 0) && (params->input_event_queue != 0) &&
         (params->state_event_queue != 0) && (params->hi2c != NULL) &&
         (params->i2c1_mutex != 0) && (params->system_init_event != 0);
}

void InputTask_Init(void* argument) {
  const InputInitParams* params = (const InputInitParams*)argument;

  if (!IsValidInitParams(params)) {
    for (;;) {
      osDelay(1);
    }
  }

  input_event_queue = params->input_event_queue;
  state_event_queue = params->state_event_queue;
  input_task_context.next_wait_ticks = osKernelGetTickCount();
  for (size_t i = 0; i < stop_button_ids.size(); i++) {
    input_task_context.track_stop_button_contexts[i] = {
        .hold_event_context =
            {
                .last_pressed_ticks = 0,
                .next_wait_ticks = 0,
                .id = stop_button_ids[i],
                .last_state = ButtonState::RELEASED,
            },
        .double_tap_event_context =
            {
                .last_pressed_ticks = 0,
                .next_wait_ticks = 0,
                .id = stop_button_ids[i],
                .last_state = ButtonState::RELEASED,
            },
    };
  }

  osEventFlagsWait(params->system_init_event, SystemInitEventFlag::Inited,
                   osFlagsWaitAll | osFlagsNoClear, osWaitForever);
  Run();
}

static void Run(void) {
  TaskStatus task_status;
  RtosMessage_InputEvent input_event;
  osStatus_t os_status;
  for (;;) {
    os_status = osMessageQueueGet(input_event_queue, &input_event, NULL,
                                  input_task_context.next_wait_ticks);
    HandleButtonHoldEvent();
    if (os_status == osOK) {
      HandleInputEvent(&input_event);
    }
  }
}

static TaskStatus HandleInputEvent(RtosMessage_InputEvent* input_event) {
  const InputEventType type = FromRtosEnumValue<InputEventType>(
      input_event->rtos_enum_value_input_event_type);
  if (type == InputEventType::MCP23017) {
    return HandleMcp23017IntEvent(&input_event->payload.mcp23017_int_event);
  } else if (type == InputEventType::ENCODER_ROTATION) {
    return HandleEncoderRotationEvent(
        &input_event->payload.encoder_rotation_event);
  } else if (type == InputEventType::ADC_CONVERSION) {
    return HandleAdcConversionEvent(&input_event->payload.adc_conversion_event);
  }
  return TASK_STATUS_OK;
}

// TODO:  버튼 입력 이벤트를 debouncing하여 잘못된 입력을 전달하지 않도록
// 검사하기
static TaskStatus HandleMcp23017IntEvent(RtosPayload_Mcp23017Event* intEvent) {
  TickType_t timestamp_ticks = intEvent->timestamp_ticks;
  Mcp23017::InterruptPin GPIO_Pin = intEvent->gpio_pin;

  Mcp23017::Address address;
  Mcp23017::InterruptSnapshot snapshot;
  Mcp23017::Status mcp23017_status;

  Mcp23017::Driver& driver = Mcp23017::Driver::GetInstance();

  driver.GetMcp23017AddressFromInterruptPin(GPIO_Pin, &address);
  if (address == Mcp23017::Address::NONE) {
    return TASK_STATUS_ERROR;
  }

  // 한 MCP23017의 두 포트를 모두 조회하여 활성화된 여러 입력핀들을 모두 처리
  mcp23017_status = driver.GetInterruptSnapshot(address, &snapshot);
  if (mcp23017_status != Mcp23017::Status::OK) {
    return TASK_STATUS_ERROR;
  }

  HandleMcp23017PortSnapshot(address, Mcp23017::Port::A, snapshot.port_a,
                             timestamp_ticks);
  HandleMcp23017PortSnapshot(address, Mcp23017::Port::B, snapshot.port_b,
                             timestamp_ticks);

  return TASK_STATUS_OK;
}

static TaskStatus HandleMcp23017PortSnapshot(
    Mcp23017::Address address, Mcp23017::Port port,
    Mcp23017::PortInterruptSnapshot snapshot, TickType_t timestamp_ticks) {
  uint8_t index = 0;
  while (snapshot.pin_mask != 0) {
    if ((snapshot.pin_mask & 0x1) != 0) {
      Mcp23017::GpioId gpio_id = FindGpioIdFromPinConfig(address, port, index);
      if (gpio_id == Mcp23017::GpioId::NONE) {
        return TASK_STATUS_ERROR;
      }
      ButtonState button_state = (snapshot.captured_pin_states & 0x1)
                                     ? ButtonState::RELEASED
                                     : ButtonState::PRESSED;
      ButtonId button_id = Mcp23017GpioToButtonMap::Get(gpio_id);

      if (TryHandleTrackStopButton(button_id, button_state, timestamp_ticks) !=
          TASK_STATUS_OK) {
        if (button_state == ButtonState::PRESSED) {
          SendButtonPayload(button_id, button_state, timestamp_ticks);
        }
      }

      UpdateButtonContext(button_id, button_state, timestamp_ticks);
      // 이벤트 처리 후 다음으로 깨어날 틱 설정
      input_task_context.next_wait_ticks =
          FindLowestNextWaitTicks(timestamp_ticks);
    }
    snapshot.pin_mask >>= 1;
    snapshot.captured_pin_states >>= 1;
    index++;
  }
  return TASK_STATUS_OK;
}

/**
 * 1. PRESSED, HOLD, DOUBLE_TAP 3개의 이벤트만 전송한다.
 * 2. 버튼이 눌렸을 때 버튼을 누른 시간이 연타 대기 시간보다 이후라면 처음으로
 * 누른 것이므로 PRESSED 이벤트를 전송한다.
 * 3. 버튼이 눌렸을 때 버튼을 누른 시간이 연타 대기 시간보다 이전이라면 이전에
 * 누른 후 더블 탭을 한 것이므로 DOUBLE TAP 이벤트를 전송한다.
 * 4. 버튼이 눌렸을 때 이전에 버튼을 뗀 상태였다면 이전에 버튼을 누른 시간을
 * 현재 시간으로 갱신한다.
 * 5. 홀드 이벤트는 항상 검사하며, 버튼을 누르고 있으며 현재 시간이 홀드 대기
 * 시간 이후라면 HOLD 이벤트를 전송한다.
 * 6. 그 외 입력과 상황의 경우 전송하지 않음.
 * 7. 해당 버튼이 아닌 경우에는 바로 PRESSED 이벤트를 전송한다.
 */
static TaskStatus TryHandleTrackStopButton(ButtonId button_id,
                                           ButtonState button_state,
                                           TickType_t timestamp_ticks) {
  for (auto& context : input_task_context.track_stop_button_contexts) {
    if (button_id != context.double_tap_event_context.id) {
      continue;
    }
    DetectButtonDoubleTapEvent(context.double_tap_event_context, button_state,
                               timestamp_ticks);
    DetectButtonHoldEvent(context.hold_event_context, button_state,
                          timestamp_ticks);
    return TASK_STATUS_OK;
  }
  return TASK_STATUS_ERROR;
}

static void DetectButtonDoubleTapEvent(ButtonEventContext& context,
                                       ButtonState state,
                                       TickType_t timestamp_ticks) {
  if (state == ButtonState::PRESSED) {
    if (context.last_state == ButtonState::RELEASED) {
      if (context.last_pressed_ticks + BUTTON_DOUBLE_TAP_THRESHOLD_TICKS >=
          timestamp_ticks) {
        SendButtonPayload(context.id, ButtonState::DOUBLE_TAP, timestamp_ticks);
        context.next_wait_ticks = osWaitForever;
      } else {
        SendButtonPayload(context.id, ButtonState::PRESSED, timestamp_ticks);
        context.next_wait_ticks =
            timestamp_ticks + BUTTON_DOUBLE_TAP_THRESHOLD_TICKS;
      }
    }
    context.last_pressed_ticks = timestamp_ticks;
  }
  context.last_state = state;
}

static void DetectButtonHoldEvent(ButtonEventContext& context,
                                  ButtonState state,
                                  TickType_t timestamp_ticks) {
  if (state == ButtonState::PRESSED &&
      context.last_state == ButtonState::RELEASED) {
    context.last_pressed_ticks = timestamp_ticks;
    context.next_wait_ticks = timestamp_ticks + BUTTON_HOLD_THRESHOLD_TICKS;
  }
  context.last_state = state;
}

static TickType_t FindLowestNextWaitTicks(TickType_t timestamp_ticks) {
  TickType_t result = osWaitForever;

  for (const auto& context : input_task_context.track_stop_button_contexts) {
    if (timestamp_ticks < context.hold_event_context.next_wait_ticks &&
        result > context.hold_event_context.next_wait_ticks) {
      result = context.hold_event_context.next_wait_ticks;
    }
    if (timestamp_ticks < context.double_tap_event_context.next_wait_ticks &&
        result > context.double_tap_event_context.next_wait_ticks) {
      result = context.double_tap_event_context.next_wait_ticks;
    }
  }
  return result;
}

static TaskStatus SendButtonPayload(ButtonId id, ButtonState state,
                                    TickType_t timestamp_ticks) {
  ButtonPayload payload = {
      .timestamp_ticks = timestamp_ticks,
      .rtos_enum_value_button_id = ToRtosEnumValue(id),
      .rtos_enum_value_button_state = ToRtosEnumValue(state),
  };
  RtosMessage_StateEvent state_event = {
      .rtos_enum_value_state_event_type =
          ToRtosEnumValue(StateEventType::BUTTON),
      .payload = {.button = payload}};
  osMessageQueuePut(state_event_queue, &state_event, 0,
                    STATE_EVENT_QUEUE_TIMEOUT_500MS);

  return TASK_STATUS_OK;
}

static void UpdateButtonContext(ButtonId id, ButtonState state,
                                TickType_t timestamp_ticks) {
  switch (id) {
    case ButtonId::ENCODER_A_PUSH:
      input_task_context.encoder_button_states[EncoderId::A] = state;
      break;
    case ButtonId::ENCODER_B_PUSH:
      input_task_context.encoder_button_states[EncoderId::B] = state;
      break;
    case ButtonId::ENCODER_C_PUSH:
      input_task_context.encoder_button_states[EncoderId::C] = state;
      break;
    case ButtonId::ENCODER_D_PUSH:
      input_task_context.encoder_button_states[EncoderId::D] = state;
      break;
    default:
      break;
  }
}

static TaskStatus HandleEncoderRotationEvent(
    RtosPayload_EncoderRotation* encoder_rotation_event) {
  int32_t delta = 1;
  if (FromRtosEnumValue<EncoderRotationDirection>(
          encoder_rotation_event->rtos_enum_value_encoder_rotation_direction) ==
      EncoderRotationDirection::COUNTER_CLOCKWISE) {
    delta = -1;
  }

  EncoderId id = FromRtosEnumValue<EncoderId>(
      encoder_rotation_event->rtos_enum_value_encoder_id);
  if (input_task_context.encoder_button_states[id] == ButtonState::PRESSED) {
    delta *= 10;
  }
  RtosMessage_StateEvent state_event = {
      .rtos_enum_value_state_event_type =
          ToRtosEnumValue(StateEventType::ENCODER_ROTATION),
      .payload = {
          .encoder_rotation = {
              .timestamp_ticks = encoder_rotation_event->timestamp_ticks,
              .rtos_enum_value_encoder_id =
                  encoder_rotation_event->rtos_enum_value_encoder_id,
              .delta = delta,
          }}};
  osMessageQueuePut(state_event_queue, &state_event, 0,
                    STATE_EVENT_QUEUE_TIMEOUT_500MS);

  return TASK_STATUS_OK;
}

static TaskStatus HandleAdcConversionEvent(
    RtosPayload_AdcConversion* adc_conversion_event) {
  RtosMessage_StateEvent state_event = {
      .rtos_enum_value_state_event_type =
          ToRtosEnumValue(StateEventType::ADC_CONVERSION),
      .payload = {.adc_conversion = {
                      .timestamp_ticks = adc_conversion_event->timestamp_ticks,
                      .rtos_enum_value_knob_id =
                          adc_conversion_event->rtos_enum_value_knob_id,
                      .adc_value = adc_conversion_event->adc_value}}};

  osMessageQueuePut(state_event_queue, &state_event, 0,
                    STATE_EVENT_QUEUE_TIMEOUT_500MS);

  return TASK_STATUS_OK;
}

static void HandleButtonHoldEvent() {
  TickType_t now = osKernelGetTickCount();
  for (auto& context : input_task_context.track_stop_button_contexts) {
    ButtonEventContext& hold_event_context = context.hold_event_context;
    if (hold_event_context.last_state == ButtonState::PRESSED &&
        hold_event_context.last_pressed_ticks + BUTTON_HOLD_THRESHOLD_TICKS <=
            now) {
      SendButtonPayload(hold_event_context.id, ButtonState::HOLD, now);
      hold_event_context.last_state = ButtonState::RELEASED;
    }
  }
}
