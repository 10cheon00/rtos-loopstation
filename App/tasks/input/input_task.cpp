#include "input_task.h"

#include "button_state.hpp"
#include "cmsis_os2.h"
#include "encoder_id.h"
#include "input_initparams.h"
#include "input_messages.h"
#include "mcp23017.hpp"
#include "mcp23017_gpio_map.hpp"
#include "mcp23017_gpio_to_button_map.hpp"
#include "state_messages.h"
#include "utils.h"

typedef struct {
  ButtonState encoder_button_state[ENCODER_ID_COUNT];
} InputTaskContext;

static osMessageQueueId_t input_event_queue;
static osMessageQueueId_t state_event_queue;

static TaskStatus HandleInputEvent(InputEvent* input_event);
static TaskStatus HandleMcp23017IntEvent(Mcp23017IntEvent* intEvent);
static TaskStatus SendButtonPayload(Mcp23017::Address address,
                                    Mcp23017::GpioId gpio_id,
                                    ButtonState button_state,
                                    TickType_t timestamp_ticks);
static TaskStatus HandleEncoderRotationEvent(
    EncoderRotationEvent* encoder_rotation_event);
static TaskStatus HandleAdcConversionEvent(
    AdcConversionEvent* adc_conversion_event);

static InputTaskContext input_task_context;

static int IsValidInitParams(const InputInitParams* params) {
  return (params != 0) &&
         (params->input_event_queue != 0 && params->state_event_queue != 0);
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

  InputTask_Run();
}

void InputTask_Run(void) {
  TaskStatus task_status;
  InputEvent input_event;
  osStatus_t os_status;
  for (;;) {
    os_status =
        osMessageQueueGet(input_event_queue, &input_event, NULL, osWaitForever);
    if (os_status == osOK) {
      task_status = HandleInputEvent(&input_event);
    }
  }
}

static TaskStatus HandleInputEvent(InputEvent* input_event) {
  if (input_event->type == INPUT_EVENT_MCP23017) {
    return HandleMcp23017IntEvent(&input_event->payload.mcp23017_int_event);
  } else if (input_event->type == INPUT_EVENT_ENCODER_ROTATION) {
    return HandleEncoderRotationEvent(
        &input_event->payload.encoder_rotation_event);
  } else if (input_event->type == INPUT_EVENT_ADC_CONVERSION) {
    return HandleAdcConversionEvent(&input_event->payload.adc_conversion_event);
  }
  return TASK_STATUS_OK;
}

// 인터럽트가 발생한 핀을 확인하고 핀 상태를 얻어낸다.
// 핀 상태에 따라 상태 관리 태스크에게 메시지를 보낸다.
// TODO:  버튼 입력 이벤트를 debouncing하여 잘못된 입력을 전달하지 않도록
// 검사하기
static TaskStatus HandleMcp23017IntEvent(Mcp23017IntEvent* intEvent) {
  TickType_t timestamp_ticks = intEvent->timestamp_ticks;
  Mcp23017::Address address;
  Mcp23017::InterruptPin GPIO_Pin = intEvent->gpio_pin;
  Mcp23017::InterruptSnapshot snapshot;
  Mcp23017::Status mcp23017_status;

  Mcp23017::GpioId gpio_id;
  ButtonState button_state;
  osStatus_t os_status;
  uint8_t index;

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

  index = 0;
  while (snapshot.port_a.pin_mask != 0) {
    if ((snapshot.port_a.pin_mask & 0x1) != 0) {
      gpio_id = FindGpioIdFromPinConfig(address, Mcp23017::Port::A, index);
      if (gpio_id == Mcp23017::GpioId::NONE) {
        return TASK_STATUS_ERROR;
      }
      button_state = (snapshot.port_a.captured_pin_states & 0x1)
                         ? ButtonState::RELEASED
                         : ButtonState::PRESSED;
      SendButtonPayload(address, gpio_id, button_state, timestamp_ticks);
    }
    snapshot.port_a.pin_mask >>= 1;
    snapshot.port_a.captured_pin_states >>= 1;
    index++;
  }
  index = 0;
  while (snapshot.port_b.pin_mask != 0) {
    if ((snapshot.port_b.pin_mask & 0x1) != 0) {
      gpio_id = FindGpioIdFromPinConfig(address, Mcp23017::Port::B, index);
      if (gpio_id == Mcp23017::GpioId::NONE) {
        return TASK_STATUS_ERROR;
      }
      button_state = (snapshot.port_b.captured_pin_states & 0x1)
                         ? ButtonState::RELEASED
                         : ButtonState::PRESSED;
      SendButtonPayload(address, gpio_id, button_state, timestamp_ticks);
    }
    snapshot.port_b.pin_mask >>= 1;
    snapshot.port_b.captured_pin_states >>= 1;
    index++;
  }
  return TASK_STATUS_OK;
}

static TaskStatus SendButtonPayload(Mcp23017::Address address,
                                    Mcp23017::GpioId gpio_id,
                                    ButtonState button_state,
                                    TickType_t timestamp_ticks) {
  ButtonId button_id = Mcp23017GpioToButtonMap::Get(gpio_id);
  ButtonPayload payload = {
      .timestamp_ticks = timestamp_ticks,
      .button_id_raw = ConvertEnumToRaw(button_id),
      .button_state_raw = ConvertEnumToRaw(button_state),
  };
  StateEvent state_event = {.type = STATE_EVENT_BUTTON,
                            .payload = {
                                .button = payload,
                            }};
  osMessageQueuePut(state_event_queue, &state_event, 0,
                    STATE_EVENT_QUEUE_TIMEOUT_500MS);

  if (button_id == ButtonId::ENCODER_A_PUSH) {
    input_task_context.encoder_button_state[ENCODER_ID_A] = button_state;
  }
  if (button_id == ButtonId::ENCODER_B_PUSH) {
    input_task_context.encoder_button_state[ENCODER_ID_B] = button_state;
  }
  if (button_id == ButtonId::ENCODER_C_PUSH) {
    input_task_context.encoder_button_state[ENCODER_ID_C] = button_state;
  }
  if (button_id == ButtonId::ENCODER_D_PUSH) {
    input_task_context.encoder_button_state[ENCODER_ID_D] = button_state;
  }
  return TASK_STATUS_OK;
}

static TaskStatus HandleEncoderRotationEvent(
    EncoderRotationEvent* encoder_rotation_event) {
  EncoderId encoder_id = encoder_rotation_event->encoder_id;
  int32_t delta = 1;
  if (encoder_rotation_event->direction == ENCODER_ROTATE_COUNTER_CLOCKWISE) {
    delta = -1;
  }
  if (input_task_context.encoder_button_state[encoder_id] ==
      ButtonState::PRESSED) {
    delta *= 10;
  }
  StateEvent state_event = {
      .type = STATE_EVENT_ENCODER_ROTATION,
      .payload = {
          .encoder_rotation = {
              .timestamp_ticks = encoder_rotation_event->timestamp_ticks,
              .encoder_id = encoder_id,
              .delta = delta,
          }}};
  osMessageQueuePut(state_event_queue, &state_event, 0,
                    STATE_EVENT_QUEUE_TIMEOUT_500MS);

  return TASK_STATUS_OK;
}

static TaskStatus HandleAdcConversionEvent(
    AdcConversionEvent* adc_conversion_event) {
  StateEvent state_event = {
      .type = STATE_EVENT_ADC_CONVERSION,
      .payload = {.adc_conversion = {
                      .timestamp_ticks = adc_conversion_event->timestamp_ticks,
                      .knob_id_raw = adc_conversion_event->knob_id_raw,
                      .adc_value = adc_conversion_event->adc_value}}};

  osMessageQueuePut(state_event_queue, &state_event, 0,
                    STATE_EVENT_QUEUE_TIMEOUT_500MS);

  return TASK_STATUS_OK;
}
