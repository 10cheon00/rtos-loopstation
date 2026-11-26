#include "state_task.h"

#include <cstdio>
#include <variant>

#include "app.h"
#include "audio_messages.h"
#include "button_state.hpp"
#include "button_to_track_action_map.hpp"
#include "cmsis_os2.h"
#include "display_messages.h"
#include "encoder_id.hpp"
#include "fx_page_group_map.hpp"
#include "fx_parameter_store.hpp"
#include "knob_id.hpp"
#include "knob_to_parameter_map.hpp"
#include "mcp23017.hpp"
#include "page_navigation_bitset.hpp"
#include "parameter_enum_value.hpp"
#include "parameter_updater.hpp"
#include "queue.h"
#include "state_event_type.hpp"
#include "state_event_variant.hpp"
#include "state_initparams.h"
#include "state_messages.h"
#include "state_task_context.hpp"
#include "system_init_event_flag.hpp"
#include "system_parameter_store.hpp"
#include "track_start_mode.hpp"
#include "track_state_machine.hpp"
#include "ui_state_machine.hpp"
#include "ui_state_map.hpp"
#include "ui_state_navigation_tree.hpp"
#include "ui_transition_map.hpp"
#include "utils.hpp"

static StateTaskContext context;

static void MakeEmptyDisplaySnapshot(RtosMessage_DisplaySnapshot& snapshot);
static void Run(void);
static TaskStatus ParseStateEvent(RtosMessage_StateEvent& state_event,
                                  StateEventVariant& state_event_variant);
static void HandleStateEvent(StateEventVariant& state_event_variant);
static void HandleButtonEvent(ButtonEvent& button_event);
static void HandleEncoderRotationEvent(
    EncoderRotationEvent& encoder_rotation_event);
static void HandleAdcConversionEvent(AdcConversionEvent& adc_conversion_event);
static bool IsButtonPressedEvent(ButtonEvent& button_event);
static void UpdateFxEditPanel();
static void TryChangePageIndex(ButtonId id);
static void TryTransitionUiStateMachine(ButtonId id);
static TaskStatus UpdateDisplaySnapshotMailbox();
static TaskStatus TryTransitionTrackStateMachine(
    TrackStateMachine::StateMachine& track_state_machine,
    ButtonEvent& button_event);
static bool IsButtonHoldEventOrDoubleTapEvent(ButtonEvent& button_event);
static void UpdateAudioEventSnapshotMailbox();
static TaskStatus FillPanelRenderPayload(RtosPayload_PanelRender& payload);
static void SetUiStateIdInPanelRenderPayload(
    RtosEnumValue& rtos_enum_value_ui_state_id);
static void SetPageNavigationBitsetInPanelRenderPayload(
    RtosEnumValue& rtos_enum_value_page_navigation_bitset);
static TaskStatus CopyPanelSlotsToPanelRenderPayload(
    RtosPayload_PageSlotRender (&page_slots)[4]);
static void SetPanelRenderPayloadPageSlotType(
    RtosPayload_PageSlotRender& payload, PageSlotVariant& slot);
static void FillPanelRenderPayloadMenu(RtosPayload_PageSlotRender& payload,
                                       MenuSlot& slot);
static TaskStatus FillPanelRenderPayloadParameter(
    RtosPayload_PageSlotRender& payload, ParameterSlot& slot);
static TaskStatus FillPanelRenderPayloadSystemParameter(
    RtosPayload_PageSlotRender& payload, ParameterSlot& slot);
static TaskStatus FillPanelRenderPayloadFxParameter(
    RtosPayload_PageSlotRender& payload, ParameterSlot& slot);
static TaskStatus FillPanelRenderPayloadNoneParameter(
    RtosPayload_PageSlotRender& payload, ParameterSlot& slot);
static TaskStatus FillDisplaySnapshotLedRenderPayload(
    RtosPayload_LedRender& led);

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
  MakeEmptyDisplaySnapshot(context.last_display_snapshot);
  xQueueOverwrite((QueueHandle_t)context.display_snapshot_mailbox,
                  &context.last_display_snapshot);

  context.track_entry[0].track_button_id_set = {
      .play_record_id = ButtonId::TRACK_1_PLAY_RECORD,
      .stop_id = ButtonId::TRACK_1_STOP,
  };

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
      HandleStateEvent(state_event_variant);

      UpdateDisplaySnapshotMailbox();
      UpdateAudioEventSnapshotMailbox();
    }
  }
}

TaskStatus ParseStateEvent(RtosMessage_StateEvent& state_event,
                           StateEventVariant& state_event_variant) {
  const StateEventType type = Utils::FromRtosEnumValue<StateEventType>(
      state_event.rtos_enum_value_state_event_type);
  if (type == StateEventType::BUTTON) {
    state_event_variant = (ButtonEvent){
        .timestamp_ticks = state_event.payload.button.timestamp_ticks,
        .id = Utils::FromRtosEnumValue<ButtonId>(
            state_event.payload.button.rtos_enum_value_button_id),
        .state = Utils::FromRtosEnumValue<ButtonState>(
            state_event.payload.button.rtos_enum_value_button_state),
    };
  } else if (type == StateEventType::ENCODER_ROTATION) {
    state_event_variant = (EncoderRotationEvent){
        .timestamp_ticks = state_event.payload.encoder_rotation.timestamp_ticks,
        .id = Utils::FromRtosEnumValue<EncoderId>(
            state_event.payload.encoder_rotation.rtos_enum_value_encoder_id),
        .delta = state_event.payload.encoder_rotation.delta,
    };
  } else if (type == StateEventType::ADC_CONVERSION) {
    state_event_variant = (AdcConversionEvent){
        .id = Utils::FromRtosEnumValue<KnobId>(
            state_event.payload.adc_conversion.rtos_enum_value_knob_id),
        .adc_value = state_event.payload.adc_conversion.adc_value,
    };
  } else {
    return TASK_STATUS_ERROR;
  }
  return TASK_STATUS_OK;
}

static void HandleStateEvent(StateEventVariant& variant) {
  std::visit(
      [](auto& event) {
        using Event = std::decay_t<decltype(event)>;

        if constexpr (std::is_same_v<Event, ButtonEvent>) {
          HandleButtonEvent(event);
        } else if constexpr (std::is_same_v<Event, EncoderRotationEvent>) {
          HandleEncoderRotationEvent(event);
        } else if constexpr (std::is_same_v<Event, AdcConversionEvent>) {
          HandleAdcConversionEvent(event);
        }
      },
      variant);
}

static void HandleButtonEvent(ButtonEvent& event) {
  if (IsButtonPressedEvent(event)) {
    if (auto fx_selection =
            ParameterUpdater::GetFxSelectionOrNulloptFromButtonEvent(event)) {
      context.fx_selection = *fx_selection;
      ParameterUpdater::ToggleFxStateFromFxSelection(context.fx_selection);
    } else {
      Page& current_page =
          context.ui_state_machine.GetCurrentState()->GetCurrentPage();
      ParameterUpdater::Update(context.fx_selection, current_page, event);
    }

    TryChangePageIndex(event.id);
    TryTransitionUiStateMachine(event.id);
    if (context.ui_state_machine.GetCurrentState()->GetId() ==
        UiStateMachine::Id::FX_EDIT) {
      UpdateFxEditPanel();
    }

    // TODO: 트랙 제어 이벤트는 버튼에 해당하는 트랙에만 전달해야 함.
    for (auto& entry : context.track_entry) {
      TryTransitionTrackStateMachine(entry.state_machine, event);
    }
  } else if (IsButtonHoldEventOrDoubleTapEvent(event)) {
    for (auto& entry : context.track_entry) {
      if (event.id == entry.track_button_id_set.stop_id) {
        // 정지 버튼을 꾹 눌렀으므로 트랙을 초기화해야함
        entry.state_machine.TryTransition(
            TrackStateMachine::ActionId::ENTER_RESET);
        break;
      }
    }
  }
}

static void HandleEncoderRotationEvent(EncoderRotationEvent& event) {
  Page& current_page =
      context.ui_state_machine.GetCurrentState()->GetCurrentPage();
  ParameterUpdater::Update(context.fx_selection, current_page, event);
  if (context.ui_state_machine.GetCurrentState()->GetId() ==
      UiStateMachine::Id::FX_EDIT) {
    UpdateFxEditPanel();
  }
}

static void HandleAdcConversionEvent(AdcConversionEvent& event) {
  ParameterUpdater::Update(event);
}

static bool IsButtonPressedEvent(ButtonEvent& event) {
  return event.state == ButtonState::PRESSED;
}

static void TryChangePageIndex(ButtonId id) {
  if (id == ButtonId::LEFT) {
    context.ui_state_machine.GetCurrentState()->DecreasePageIndex();
  } else if (id == ButtonId::RIGHT) {
    context.ui_state_machine.GetCurrentState()->IncreasePageIndex();
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
      SlotIndex index = Utils::ToSlotIndex(id);
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

static void UpdateFxEditPanel() {
  Store::FxParameterStore::GetInstance().WithParameterLocked(
      context.fx_selection.section, context.fx_selection.slot,
      Fx::ParameterId::FX_SYSTEM_SELECTED_FX, [](auto& fx_parameter) {
        Fx::Type selected_fx = std::get<ParameterModel::FxSelectedFxEnumValue>(
                                   fx_parameter.GetValueVariant())
                                   .value;

        // 현재 fx_selection에 따라 페이지 그룹, 라벨을 업데이트함
        // 업데이트된 페이지 그룹과 라벨은 스냅샷에 복사되어 디스플레이 태스크에
        // 전달됨
        context.ui_state_machine.UpdatePanel(context.fx_selection.section,
                                             context.fx_selection.slot,
                                             selected_fx);
      });
}

static bool IsButtonHoldEventOrDoubleTapEvent(ButtonEvent& event) {
  return event.state == ButtonState::HOLD ||
         event.state == ButtonState::DOUBLE_TAP;
}

static TaskStatus UpdateDisplaySnapshotMailbox() {
  RtosMessage_DisplaySnapshot snapshot{};
  MakeEmptyDisplaySnapshot(snapshot);

  TaskStatus panel_copied = FillPanelRenderPayload(snapshot.panel);
  TaskStatus led_copied = FillDisplaySnapshotLedRenderPayload(snapshot.led);
  if (panel_copied == TASK_STATUS_OK && led_copied == TASK_STATUS_OK) {
    context.last_display_snapshot = snapshot;
  }

  xQueueOverwrite((QueueHandle_t)context.display_snapshot_mailbox,
                  &context.last_display_snapshot);
  return TASK_STATUS_OK;
}

static void MakeEmptyDisplaySnapshot(RtosMessage_DisplaySnapshot& snapshot) {
  snapshot.panel.rtos_enum_value_ui_state =
      Utils::ToRtosEnumValue(UiStateMachine::Id::NONE);
  snapshot.panel.rtos_enum_value_page_navigation_bitset =
      PageNavigationBitset{}.ToRtosEnumValue();
  for (auto& slot : snapshot.panel.page_slots) {
    slot.rtos_enum_value_page_slot_type =
        Utils::ToRtosEnumValue(PageSlotType::NONE);
    slot.data.parameter.rtos_parameter_copy.rtos_enum_value_parameter_type =
        Utils::ToRtosEnumValue(ParameterType::NONE);
    slot.data.parameter.label = "";
  }
  snapshot.led.rtos_parameter_copy_ifx_a_state.rtos_enum_value_parameter_type =
      Utils::ToRtosEnumValue(ParameterType::NONE);
  snapshot.led.rtos_parameter_copy_tfx_a_state.rtos_enum_value_parameter_type =
      Utils::ToRtosEnumValue(ParameterType::NONE);
  for (auto& state : snapshot.led.rtos_enum_value_track_states) {
    state = Utils::ToRtosEnumValue(TrackStateMachine::Id::NONE);
  }
}

static TaskStatus FillPanelRenderPayload(RtosPayload_PanelRender& payload) {
  SetUiStateIdInPanelRenderPayload(payload.rtos_enum_value_ui_state);
  const auto& label = context.ui_state_machine.GetCurrentState()->GetLabel();
  std::snprintf(payload.label, sizeof(payload.label), "%s", label.data());
  SetPageNavigationBitsetInPanelRenderPayload(
      payload.rtos_enum_value_page_navigation_bitset);
  return CopyPanelSlotsToPanelRenderPayload(payload.page_slots);
}

static void SetUiStateIdInPanelRenderPayload(
    RtosEnumValue& rtos_enum_value_ui_state_id) {
  rtos_enum_value_ui_state_id = Utils::ToRtosEnumValue(
      context.ui_state_machine.GetCurrentState()->GetId());
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

static TaskStatus CopyPanelSlotsToPanelRenderPayload(
    RtosPayload_PageSlotRender (&page_slots)[4]) {
  Page& page = context.ui_state_machine.GetCurrentState()->GetCurrentPage();
  for (std::uint8_t i = 0; i < static_cast<std::uint8_t>(SlotIndex::COUNT);
       i++) {
    SlotIndex slot_index = static_cast<SlotIndex>(i);
    PageSlotVariant& slot = page.GetAt(slot_index);
    SetPanelRenderPayloadPageSlotType(page_slots[i], slot);

    if (std::holds_alternative<MenuSlot>(slot)) {
      FillPanelRenderPayloadMenu(page_slots[i], std::get<MenuSlot>(slot));
    } else if (std::holds_alternative<ParameterSlot>(slot)) {
      if (FillPanelRenderPayloadParameter(
              page_slots[i], std::get<ParameterSlot>(slot)) != TASK_STATUS_OK) {
        return TASK_STATUS_ERROR;
      }
    }
  }
  return TASK_STATUS_OK;
}

static void SetPanelRenderPayloadPageSlotType(
    RtosPayload_PageSlotRender& payload, PageSlotVariant& slot) {
  payload.rtos_enum_value_page_slot_type =
      Utils::ToRtosEnumValue(GetPageSlotType(slot));
}

static void FillPanelRenderPayloadMenu(RtosPayload_PageSlotRender& payload,
                                       MenuSlot& slot) {
  payload.data.menu = (RtosPayload_MenuRender){
      .rtos_enum_value16_menu_icon_encoding =
          Utils::ToRtosEnumValue(slot.GetIconEncoding()),
      .label = slot.GetLabel()};
}

static TaskStatus FillPanelRenderPayloadParameter(
    RtosPayload_PageSlotRender& payload, ParameterSlot& slot) {
  if (std::holds_alternative<SystemParameterRef>(slot.GetParameterRef())) {
    return FillPanelRenderPayloadSystemParameter(payload, slot);
  } else if (std::holds_alternative<FxParameterRef>(slot.GetParameterRef())) {
    return FillPanelRenderPayloadFxParameter(payload, slot);
  } else {
    return FillPanelRenderPayloadNoneParameter(payload, slot);
  }
}

static TaskStatus FillPanelRenderPayloadSystemParameter(
    RtosPayload_PageSlotRender& payload, ParameterSlot& slot) {
  bool copied = false;
  SystemParameterRef& system_parameter_ref =
      std::get<SystemParameterRef>(slot.GetParameterRef());
  Store::SystemParameterStore::GetInstance().WithParameterLocked(
      system_parameter_ref.id,
      [&rtos_parameter_copy = payload.data.parameter.rtos_parameter_copy,
       &copied](auto& parameter) {
        parameter.ToRtosParameterCopy(rtos_parameter_copy);
        copied = true;
      });
  payload.data.parameter.label = slot.GetLabel();
  if (!copied) {
    return TASK_STATUS_ERROR;
  }
  return TASK_STATUS_OK;
}

static TaskStatus FillPanelRenderPayloadFxParameter(
    RtosPayload_PageSlotRender& payload, ParameterSlot& slot) {
  bool copied = false;
  FxParameterRef& fx_parameter_ref =
      std::get<FxParameterRef>(slot.GetParameterRef());
  Fx::ParameterId fx_parameter_id = fx_parameter_ref.id;
  Store::FxParameterStore::GetInstance().WithParameterLocked(
      context.fx_selection.section, context.fx_selection.slot, fx_parameter_id,
      [&rtos_parameter_copy = payload.data.parameter.rtos_parameter_copy,
       &copied](auto& fx_parameter) {
        fx_parameter.ToRtosParameterCopy(rtos_parameter_copy);
        copied = true;
      });
  payload.data.parameter.label = slot.GetLabel();
  if (!copied) {
    return TASK_STATUS_ERROR;
  }
  return TASK_STATUS_OK;
}

static TaskStatus FillPanelRenderPayloadNoneParameter(
    RtosPayload_PageSlotRender& payload, ParameterSlot& slot) {
  payload.data.parameter.rtos_parameter_copy.rtos_enum_value_parameter_type =
      Utils::ToRtosEnumValue(ParameterType::NONE);
  payload.data.parameter.label = slot.GetLabel();
  return TASK_STATUS_OK;
}

static TaskStatus FillDisplaySnapshotLedRenderPayload(
    RtosPayload_LedRender& led) {
  bool ifx_copied = false;
  bool tfx_copied = false;
  // TODO:
  // Slot::A에 대해서만 복사를 하고 있음
  Store::FxParameterStore::GetInstance().WithParameterLocked(
      Fx::Section::IFX, Fx::Slot::A, Fx::ParameterId::FX_SYSTEM_FX_STATE,
      [&rtos_parameter_copy = led.rtos_parameter_copy_ifx_a_state,
       &ifx_copied](auto& fx_parameter) {
        fx_parameter.ToRtosParameterCopy(rtos_parameter_copy);
        ifx_copied = true;
      });
  Store::FxParameterStore::GetInstance().WithParameterLocked(
      Fx::Section::TFX, Fx::Slot::A, Fx::ParameterId::FX_SYSTEM_FX_STATE,
      [&rtos_parameter_copy = led.rtos_parameter_copy_tfx_a_state,
       &tfx_copied](auto& fx_parameter) {
        fx_parameter.ToRtosParameterCopy(rtos_parameter_copy);
        tfx_copied = true;
      });

  for (std::size_t i = 0; i < context.track_entry.size(); i++) {
    led.rtos_enum_value_track_states[i] = Utils::ToRtosEnumValue(
        context.track_entry[i].state_machine.GetCurrentState()->GetId());
  }
  if (!ifx_copied || !tfx_copied) {
    return TASK_STATUS_ERROR;
  }
  return TASK_STATUS_OK;
}

static void UpdateAudioEventSnapshotMailbox() {
  RtosMessage_AudioEventSnapshot audio_event_snapshot = {};

  for (std::size_t i = 0; i < context.track_entry.size(); i++) {
    audio_event_snapshot.rtos_enum_value_track_states[i] =
        Utils::ToRtosEnumValue(
            context.track_entry[i].state_machine.GetCurrentState()->GetId());
    // TODO: 스냅샷에 각 트랙의 볼륨을 읽어 넣어야 함.
    ParameterModel::UnsignedRangeValue track_volume =
        std::get<ParameterModel::UnsignedRangeValue>(
            Store::SystemParameterStore::GetInstance()
                .Get(ParameterModel::Id::TRACK_1_VOLUME)
                .GetValueVariant());
    audio_event_snapshot.rtos_parameter_track_volumes[i] = track_volume.current;
  }

  xQueueOverwrite(context.audio_event_snapshot_mailbox, &audio_event_snapshot);
}
