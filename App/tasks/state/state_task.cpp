#include "state_task.h"

#include <variant>

#include "FreeRTOS.h"
#include "app.h"
#include "audio_messages.h"
#include "button_state.hpp"
#include "button_to_track_action_map.hpp"
#include "cmsis_os2.h"
#include "display_messages.h"
#include "encoder_id.hpp"
#include "knob_id.hpp"
#include "knob_to_parameter_map.hpp"
#include "loopstation_parameter_store.hpp"
#include "mcp23017.hpp"
#include "page_navigation_bitset.hpp"
#include "queue.h"
#include "state_event_type.hpp"
#include "state_initparams.h"
#include "state_messages.h"
#include "system_init_event_flag.hpp"
#include "track_config.h"
#include "track_state_machine.hpp"
#include "ui_state_machine.hpp"
#include "ui_state_navigation_tree.hpp"
#include "ui_state_pointer_map.hpp"
#include "ui_transition_map.hpp"
#include "utils.h"

struct ButtonEvent {
  TickType_t timestamp_ticks;
  ButtonId id;
  ButtonState state;
};

struct EncoderRotationEvent {
  TickType_t timestamp_ticks;
  EncoderId id;
  int32_t delta;
};

struct AdcConversionEvent {
  KnobId id;
  uint16_t adc_value;
};

using StateEventVariant =
    std::variant<ButtonEvent, EncoderRotationEvent, AdcConversionEvent>;

struct StateTaskContext {
  osMessageQueueId_t state_event_queue;
  osMessageQueueId_t display_snapshot_mailbox;
  osMessageQueueId_t audio_event_snapshot_mailbox;

  UiStateMachine::Context ui_state_machine_context;
  UiStateMachine::StateMachine ui_state_machine{ui_state_machine_context,
                                                UiStateMachine::Id::HOME};

  std::array<TrackStateMachine::Context, static_cast<std::size_t>(TRACK_COUNT)>
      track_state_machine_contexts;
  std::array<TrackStateMachine::StateMachine,
             static_cast<std::size_t>(TRACK_COUNT)>
      track_state_machines{TrackStateMachine::StateMachine{
          track_state_machine_contexts[0], TrackStateMachine::Id::IDLE}};
};

static StateTaskContext context;

static void Run(void);
static TaskStatus ParseStateEvent(RtosMessage_StateEvent& state_event,
                                  StateEventVariant& state_event_variant);
static TaskStatus TryUpdateParameter(StateEventVariant& state_event_variant);
static TaskStatus TryUpdateParameterFromButton(ButtonEvent& event);
static Parameter& GetParameterFromCurrentPageAt(SlotIndex index);
static TaskStatus TryUpdateParameterFromEncoderRotation(
    EncoderRotationEvent& event);
static TaskStatus TryUpdateParameterFromAdc(AdcConversionEvent& event);
static bool IsButtonPressedEvent(StateEventVariant& state_event_variant);
static TaskStatus HandlePanelControlButtonEvent(ButtonEvent& event);
static void TryChangePageIndex(ButtonId id);
static void TryTransitionUiStateMachine(ButtonId id);
static TaskStatus UpdateDisplaySnapshotMailbox();
static TaskStatus TryTransitionTrackStateMachine(
    TrackStateMachine::StateMachine& track_state_machine,
    ButtonEvent& button_event);
static void UpdateAudioEventSnapshotMailbox();
static void FillPanelRenderPayload(RtosPayload_PanelRender& payload);
static void SetUiStateIdInPanelRenderPayload(
    RtosEnumValue& rtos_enum_value_ui_state_id);
static void SetPageNavigationBitsetInPanelRenderPayload(
    RtosEnumValue& rtos_enum_value_page_navigation_bitset);
static void CopyPanelSlotsToPanelRenderPayload(
    RtosPayload_PageSlotRender (&page_slots)[4]);
static void FillDisplaySnapshotLedRenderPayload(RtosPayload_LedRender& led);

static int IsValidInitParams(const StateInitParams* params) {
  return (params != 0) && (params->state_event_queue != 0) &&
         (params->audio_event_snapshot_mailbox != 0) &&
         (params->display_snapshot_mailbox != 0) && (params->hi2c != NULL) &&
         (params->i2c_mutex != 0) && (params->system_init_event != 0);
}

void StateTask_Init(void* argument) {
  const StateInitParams* params = (const StateInitParams*)argument;

  if (!IsValidInitParams(params)) {
    for (;;) {
      osDelay(1);
    }
  }

  context.state_event_queue = params->state_event_queue;
  context.display_snapshot_mailbox = params->display_snapshot_mailbox;
  context.audio_event_snapshot_mailbox = params->audio_event_snapshot_mailbox;

  osEventFlagsWait(params->system_init_event, SystemInitEventFlag::Inited,
                   osFlagsWaitAll | osFlagsNoClear, osWaitForever);
  Run();
}

void Run(void) {
  RtosMessage_StateEvent state_event;
  osStatus_t os_status;
  TaskStatus task_status;
  StateEventVariant state_event_variant;
  for (;;) {
    // WaitForStateEvent
    os_status = osMessageQueueGet(context.state_event_queue, &state_event, NULL,
                                  osWaitForever);
    if (os_status == osOK) {
      task_status = ParseStateEvent(state_event, state_event_variant);
      if (task_status != TASK_STATUS_OK) {
        continue;
      }
      TryUpdateParameter(state_event_variant);

      if (IsButtonPressedEvent(state_event_variant)) {
        // UpdateStateMachines
        ButtonEvent& button_event = std::get<ButtonEvent>(state_event_variant);
        HandlePanelControlButtonEvent(button_event);
        for (uint8_t i = 0; i < TRACK_COUNT; i++) {
          TryTransitionTrackStateMachine(context.track_state_machines[i],
                                         button_event);
        }
      }
      // UpdatePanel
      UpdateDisplaySnapshotMailbox();
      UpdateAudioEventSnapshotMailbox();
    }
  }
}

TaskStatus ParseStateEvent(RtosMessage_StateEvent& state_event,
                           StateEventVariant& state_event_variant) {
  const StateEventType type = FromRtosEnumValue<StateEventType>(
      state_event.rtos_enum_value_state_event_type);
  if (type == StateEventType::BUTTON) {
    state_event_variant = (ButtonEvent){
        .timestamp_ticks = state_event.payload.button.timestamp_ticks,
        .id = FromRtosEnumValue<ButtonId>(
            state_event.payload.button.rtos_enum_value_button_id),
        .state = FromRtosEnumValue<ButtonState>(
            state_event.payload.button.rtos_enum_value_button_state),
    };
  } else if (type == StateEventType::ENCODER_ROTATION) {
    state_event_variant = (EncoderRotationEvent){
        .timestamp_ticks = state_event.payload.button.timestamp_ticks,
        .id = FromRtosEnumValue<EncoderId>(
            state_event.payload.encoder_rotation.rtos_enum_value_encoder_id),
        .delta = state_event.payload.encoder_rotation.delta,
    };
  } else if (type == StateEventType::ADC_CONVERSION) {
    state_event_variant = (AdcConversionEvent){
        .id = FromRtosEnumValue<KnobId>(
            state_event.payload.adc_conversion.rtos_enum_value_knob_id),
        .adc_value = state_event.payload.adc_conversion.adc_value,
    };
  } else {
    return TASK_STATUS_ERROR;
  }
  return TASK_STATUS_OK;
}

static TaskStatus TryUpdateParameter(StateEventVariant& variant) {
  if (std::holds_alternative<ButtonEvent>(variant)) {
    return TryUpdateParameterFromButton(std::get<ButtonEvent>(variant));
  } else if (std::holds_alternative<EncoderRotationEvent>(variant)) {
    return TryUpdateParameterFromEncoderRotation(
        std::get<EncoderRotationEvent>(variant));
  } else if (std::holds_alternative<AdcConversionEvent>(variant)) {
    // TODO:
    // ADC 입력에 대한 파라미터 값 변경 기능 구현하기
    return TryUpdateParameterFromAdc(std::get<AdcConversionEvent>(variant));
  }
  return TASK_STATUS_ERROR;
}

/**
 * 버튼 입력은 IFX/TFX 토글, 엔코더 버튼만 파라미터 값을 변경한다.
 * */
static TaskStatus TryUpdateParameterFromButton(ButtonEvent& event) {
  if (event.state != ButtonState::PRESSED) {
    return TASK_STATUS_ERROR;
  }

  switch (event.id) {
    case ButtonId::ENCODER_A_PUSH:
    case ButtonId::ENCODER_B_PUSH:
    case ButtonId::ENCODER_C_PUSH:
    case ButtonId::ENCODER_D_PUSH: {
      SlotIndex index = ToSlotIndex(event.id);
      Parameter& parameter = GetParameterFromCurrentPageAt(index);
      if (parameter.GetType() == ParameterType::TOGGLE) {
        parameter.Toggle();
        return TASK_STATUS_OK;
      }
    } break;
    case ButtonId::IFX_A_TOGGLE: {
      Parameter& parameter =
          LoopstationStore::GetParameter(ParameterId::IFX_A_STATE);
      parameter.Toggle();
    } break;
    case ButtonId::TFX_A_TOGGLE: {
      Parameter& parameter =
          LoopstationStore::GetParameter(ParameterId::TFX_A_STATE);
      parameter.Toggle();
    } break;
    default:
      return TASK_STATUS_ERROR;
  }
  return TASK_STATUS_OK;
}

static Parameter& GetParameterFromCurrentPageAt(SlotIndex index) {
  Page& current_page =
      context.ui_state_machine.GetCurrentState()->GetCurrentPage();

  if (current_page.IsTypeAt<ParameterSlot>(index)) {
    ParameterId parameter_id =
        std::get<ParameterSlot>(current_page.GetAt(index)).GetParameterId();
    return LoopstationStore::GetParameter(parameter_id);
  }
  return LoopstationStore::GetParameter(ParameterId::NONE);
}

/**
 * 엔코더 입력은 패널에 엔코더와 매핑된 파라미터가 있어야 하며 그 파라미터가
 * 토글형이라면 버튼입력과 회전입력으로 토글을 수행하고 노브형이라면
 * 회전입력을 받아 값을 변경한다.
 */
static TaskStatus TryUpdateParameterFromEncoderRotation(
    EncoderRotationEvent& event) {
  SlotIndex index = ToSlotIndex(event.id);
  Parameter& parameter = GetParameterFromCurrentPageAt(index);
  parameter.Add(event.delta);
  return TASK_STATUS_OK;
}

/**
 * ADC입력은 16비트 해상도의 반환값을 파라미터 자료형에 맞도록 스케일한다.
 * 이 입력을 곧바로 FX 파라미터에 적용하지 않고 별도의 파라미터에 저장한다음,
 * 저장된 값을 FX 파라미터에 적용한다.
 */
static TaskStatus TryUpdateParameterFromAdc(AdcConversionEvent& event) {
  Parameter& parameter =
      LoopstationStore::GetParameter(KnobToParameterMap::Get(event.id));
  parameter.Set(MapRangeLinear(event.adc_value,
                               std::numeric_limits<std::uint16_t>::min(),
                               std::numeric_limits<std::uint16_t>::max(),
                               parameter.GetMin(), parameter.GetMax()));
  return TASK_STATUS_OK;
}

static bool IsButtonPressedEvent(StateEventVariant& state_event_variant) {
  if (!std::holds_alternative<ButtonEvent>(state_event_variant)) {
    return false;
  }
  return std::get<ButtonEvent>(state_event_variant).state ==
         ButtonState::PRESSED;
}

static TaskStatus HandlePanelControlButtonEvent(ButtonEvent& event) {
  TryChangePageIndex(event.id);
  TryTransitionUiStateMachine(event.id);

  return TASK_STATUS_OK;
}

static void TryChangePageIndex(ButtonId id) {
  if (id == ButtonId::LEFT) {
    context.ui_state_machine.GetCurrentState()->IncreasePageIndex();
  } else if (id == ButtonId::RIGHT) {
    context.ui_state_machine.GetCurrentState()->DecreasePageIndex();
  }
}

static void TryTransitionUiStateMachine(ButtonId id) {
  UiStateMachine::Id next_ui_state_id = UiStateMachine::Id::NONE;

  switch (id) {
    case ButtonId::EXIT:
      next_ui_state_id = UiStateNavigationTree_GetParent(
          context.ui_state_machine.GetCurrentState()->GetId());
      break;
    case ButtonId::ENCODER_A_PUSH:
    case ButtonId::ENCODER_B_PUSH:
    case ButtonId::ENCODER_C_PUSH:
    case ButtonId::ENCODER_D_PUSH: {
      SlotIndex index = ToSlotIndex(id);
      PageSlotVariant& page_slot_variant =
          context.ui_state_machine.GetCurrentState()->GetCurrentPage().GetAt(
              index);
      if (std::holds_alternative<MenuSlot>(page_slot_variant)) {
        next_ui_state_id = std::get<MenuSlot>(page_slot_variant).GetUiStateId();
      }
    } break;
    default:
      next_ui_state_id = UiTransitionMap::Get(id);
  }
  if (next_ui_state_id != UiStateMachine::Id::NONE) {
    context.ui_state_machine.TryTransition(next_ui_state_id);
  }
}

static TaskStatus UpdateDisplaySnapshotMailbox() {
  RtosMessage_DisplaySnapshot snapshot;

  FillPanelRenderPayload(snapshot.panel);
  FillDisplaySnapshotLedRenderPayload(snapshot.led);

  xQueueOverwrite((QueueHandle_t)context.display_snapshot_mailbox, &snapshot);
  return TASK_STATUS_OK;
}

/**
 * 버튼에 매핑된 전이 이벤트가 있는지 확인 후 전이
 */
static TaskStatus TryTransitionTrackStateMachine(
    TrackStateMachine::StateMachine& track_state_machine,
    ButtonEvent& button_event) {
  TrackStateMachine::ActionId action_id =
      ButtonToTrackActionMap::Get(button_event.id);
  if (action_id == TrackStateMachine::ActionId::NONE) {
    return TASK_STATUS_ERROR;
  }

  track_state_machine.TryTransition(action_id);
  return TASK_STATUS_OK;
}

static void UpdateAudioEventSnapshotMailbox() {
  RtosMessage_AudioEventSnapshot audio_event_snapshot = {

      // TODO:
      // IFX, TFX 파라미터값 복사하기
  };

  for (uint8_t i = 0; i < TRACK_COUNT; i++) {
    audio_event_snapshot.rtos_enum_value_track_states[i] = ToRtosEnumValue(
        context.track_state_machines[i].GetCurrentState()->GetId());
    audio_event_snapshot.rtos_parameter_track_volumes[i] =
        LoopstationStore::GetParameter(ParameterId::TRACK_1_VOLUME)
            .GetCurrentForRtos();
  }

  xQueueOverwrite((QueueHandle_t)context.audio_event_snapshot_mailbox,
                  &audio_event_snapshot);
}

static void FillPanelRenderPayload(RtosPayload_PanelRender& payload) {
  SetUiStateIdInPanelRenderPayload(payload.rtos_enum_value_ui_state);
  SetPageNavigationBitsetInPanelRenderPayload(
      payload.rtos_enum_value_page_navigation_bitset);
  CopyPanelSlotsToPanelRenderPayload(payload.page_slots);
}

static void SetUiStateIdInPanelRenderPayload(
    RtosEnumValue& rtos_enum_value_ui_state_id) {
  rtos_enum_value_ui_state_id =
      ToRtosEnumValue(context.ui_state_machine.GetCurrentState()->GetId());
}

static void SetPageNavigationBitsetInPanelRenderPayload(
    RtosEnumValue& rtos_enum_value_page_navigation_bitset) {
  PageNavigationBitset bitset;
  if (context.ui_state_machine.GetCurrentState()->CanDecreasePageIndex()) {
    bitset |= PageNavigation::LEFT_ARROW;
  }
  if (context.ui_state_machine.GetCurrentState()->CanIncreasePageIndex()) {
    bitset |= PageNavigation::RIGHT_ARROW;
  }
  rtos_enum_value_page_navigation_bitset = bitset.ToRtosEnumValue();
}

static void CopyPanelSlotsToPanelRenderPayload(
    RtosPayload_PageSlotRender (&page_slots)[4]) {
  Page& page = context.ui_state_machine.GetCurrentState()->GetCurrentPage();
  for (std::uint8_t i = 0; i < static_cast<std::uint8_t>(SlotIndex::COUNT);
       i++) {
    SlotIndex slot_index = static_cast<SlotIndex>(i);
    PageSlotVariant& page_slot_variant = page.GetAt(slot_index);
    page_slots[i].rtos_enum_value_page_slot_type =
        ToRtosEnumValue(GetPageSlotType(page_slot_variant));

    if (std::holds_alternative<MenuSlot>(page_slot_variant)) {
      MenuSlot& menu_slot = std::get<MenuSlot>(page_slot_variant);
      page_slots[i].data.menu = (RtosPayload_MenuRender){
          .rtos_enum_value16_menu_icon_encoding =
              ToRtosEnumValue(menu_slot.GetIconEncoding()),
          .label = menu_slot.GetLabel()};
    } else if (std::holds_alternative<ParameterSlot>(page_slot_variant)) {
      ParameterSlot& parameter_slot =
          std::get<ParameterSlot>(page_slot_variant);
      Parameter parameter =
          LoopstationStore::GetParameter(parameter_slot.GetParameterId());
      parameter.ToRtosParameterCopy(
          page_slots[i].data.parameter.rtos_parameter_copy);
      page_slots[i].data.parameter.label = parameter_slot.GetLabel();
    }
  }
}

static void FillDisplaySnapshotLedRenderPayload(RtosPayload_LedRender& led) {
  LoopstationStore::GetParameter(ParameterId::IFX_A_STATE)
      .ToRtosParameterCopy(led.ifx_a_state);
  LoopstationStore::GetParameter(ParameterId::TFX_A_STATE)
      .ToRtosParameterCopy(led.tfx_a_state);

  for (uint8_t i = 0; i < TRACK_COUNT; i++) {
    led.rtos_enum_value_track_states[i] = ToRtosEnumValue(
        context.track_state_machines[i].GetCurrentState()->GetId());
  }
}
