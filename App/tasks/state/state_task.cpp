#include "state_task.h"

#include "FreeRTOS.h"
#include "app.h"
#include "button_state.hpp"
#include "button_to_track_action_map.hpp"
#include "cmsis_os2.h"
#include "display_messages.h"
#include "encoder_id.h"
#include "global_ui_transition_config_table.hpp"
#include "loopstation_parameter_store.hpp"
#include "queue.h"
#include "state_initparams.h"
#include "state_messages.h"
#include "system_state_machine.hpp"
#include "track_config.h"
#include "track_state_machine.hpp"
#include "ui_state_config_table.hpp"
#include "ui_state_machine.hpp"
#include "ui_state_navigation_tree.hpp"
#include "utils.h"

static StateTaskContext state_task_context;

static osMessageQueueId_t state_event_queue = 0;
static osMessageQueueId_t display_snapshot_mailbox = 0;

static SystemStateMachine::Context system_state_machine_context;
static SystemStateMachine::StateMachine system_state_machine{
    system_state_machine_context, SystemStateMachine::Id::NOT_INITED};

static UiStateMachine::Context ui_state_machine_context;
static UiStateMachine::StateMachine ui_state_machine{ui_state_machine_context,
                                                     UiStateMachine::Id::HOME};

static std::array<TrackStateMachine::Context,
                  static_cast<std::size_t>(TRACK_COUNT)>
    track_state_machine_contexts;
static std::array<TrackStateMachine::StateMachine,
                  static_cast<std::size_t>(TRACK_COUNT)>
    track_state_machines{TrackStateMachine::StateMachine{
        track_state_machine_contexts[0], TrackStateMachine::Id::IDLE}};

static TaskStatus TryUpdateParameter(StateEvent& state_event);
static TaskStatus TryUpdateParameterFromButton(ButtonPayload& button_payload);
static TaskStatus TryUpdateParameterFromEncoderRotation(
    EncoderRotationPayload& encoder_rotation_payload);
static TaskStatus TryUpdateParameterFromAdc(StateEvent& state_event);
static TaskStatus TryTransitionUiStateMachine(StateEvent& state_event);
static TaskStatus UpdateDisplaySnapshotMailbox();
static TaskStatus TryTransitionTrackStateMachine(
    TrackStateMachine::StateMachine& track_state_machine,
    StateEvent& state_event);

static int IsValidInitParams(const StateInitParams* params) {
  return (params != 0) && (params->state_event_queue != 0) &&
         (params->display_snapshot_mailbox != 0);
}

void StateTask_Init(void* argument) {
  const StateInitParams* params = (const StateInitParams*)argument;

  if (!IsValidInitParams(params)) {
    for (;;) {
      osDelay(1);
    }
  }

  state_event_queue = params->state_event_queue;
  display_snapshot_mailbox = params->display_snapshot_mailbox;

  StateTask_Run();
}

void StateTask_Run(void) {
  StateEvent state_event;
  osStatus_t os_status;
  TaskStatus task_status;

  for (;;) {
    if (system_state_machine.GetCurrentState()->GetId() ==
        SystemStateMachine::Id::ERROR) {
      // TODO:
      // 시스템 검증 결과에 오류가 있으면 이를 사용자에게 알려야 함.
      // 지금은 임시로 그냥 무한루프 처리를 했음
      for (;;) {
        osDelay(1);
      }
    }
    os_status =
        osMessageQueueGet(state_event_queue, &state_event, NULL, osWaitForever);
    if (os_status == osOK) {
      TryUpdateParameter(state_event);
      TryTransitionUiStateMachine(state_event);
      for (uint8_t i = 0; i < TRACK_COUNT; i++) {
        TryTransitionTrackStateMachine(track_state_machines[i], state_event);
      }
      if (task_status != TASK_STATUS_OK) {
        // TODO:
        // 처리 실패에 대한 예외처리 구현하기
      }
      UpdateDisplaySnapshotMailbox();
    }
  }
}

static TaskStatus TryUpdateParameter(StateEvent& state_event) {
  if (state_event.type == STATE_EVENT_BUTTON) {
    return TryUpdateParameterFromButton(state_event.payload.button);
  } else if (state_event.type == STATE_EVENT_ENCODER_ROTATION) {
    return TryUpdateParameterFromEncoderRotation(
        state_event.payload.encoder_rotation);
  } else {
    // TODO:
    // ADC 입력에 대한 파라미터 값 변경 기능 구현하기
    return TryUpdateParameterFromAdc(state_event);
  }
}

/**
 * 버튼 입력은 IFX/TFX 토글, 엔코더 버튼만 파라미터 값을 변경한다.
 * */
static TaskStatus TryUpdateParameterFromButton(ButtonPayload& button_payload) {
  ParameterId parameter_id;

  ButtonState button_state;
  if (!ConvertEnumRawToEnum<ButtonState>(button_payload.button_state_raw,
                                         &button_state)) {
    return TASK_STATUS_ERROR;
  }
  if (button_state != ButtonState::PRESSED) {
    return TASK_STATUS_ERROR;
  }

  ButtonId id;
  if (!ConvertEnumRawToEnum<ButtonId>(button_payload.button_id_raw, &id)) {
    return TASK_STATUS_ERROR;
  }

  if (id != ButtonId::IFX_A_TOGGLE && id != ButtonId::TFX_A_TOGGLE &&
      id != ButtonId::ENCODER_A_PUSH) {
    return TASK_STATUS_ERROR;
  }

  if (id == ButtonId::ENCODER_A_PUSH) {
    // TODO:
    // Encoder_A~D 모두 처리 가능하게 해야함
    const UiStateMachine::PanelSlot& panel_slot =
        ui_state_machine.GetCurrentState()
            ->GetCurrentPage()[UI_STATE_SLOT_INDEX_A];
    parameter_id = panel_slot.data.parameter.id;
    if (parameter_id == ParameterId::NONE) {
      return TASK_STATUS_ERROR;
    }

    Parameter& parameter = LoopstationStore::GetParameter(parameter_id);
    if (parameter.type == PARAMETER_TYPE_TOGGLE) {
      Parameter_ToggleValue(&parameter);
      return TASK_STATUS_OK;
    }
  } else if (id == ButtonId::IFX_A_TOGGLE) {
    Parameter& parameter =
        LoopstationStore::GetParameter(ParameterId::IFX_A_STATE);
    Parameter_ToggleValue(&parameter);
    return TASK_STATUS_OK;
  } else if (id == ButtonId::TFX_A_TOGGLE) {
    Parameter& parameter =
        LoopstationStore::GetParameter(ParameterId::TFX_A_STATE);
    Parameter_ToggleValue(&parameter);
    return TASK_STATUS_OK;
  }

  return TASK_STATUS_ERROR;
}

TaskStatus TryUpdateParameterFromEncoderRotation(
    EncoderRotationPayload& encoder_rotation_payload) {
  ParameterId parameter_id;

  EncoderId encoder_id;

  encoder_id = encoder_rotation_payload.encoder_id;
  const UiStateMachine::PanelSlot& panel_slot =
      ui_state_machine.GetCurrentState()->GetCurrentPage()[encoder_id];
  if (panel_slot.type == PANEL_SLOT_TYPE_MENU) {
    return TASK_STATUS_OK;
  }

  parameter_id = panel_slot.data.parameter.id;
  if (parameter_id == ParameterId::NONE) {
    return TASK_STATUS_ERROR;
  }
  Parameter& parameter = LoopstationStore::GetParameter(parameter_id);
  if (parameter.type == PARAMETER_TYPE_TOGGLE) {
    Parameter_ToggleValue(&parameter);
    return TASK_STATUS_OK;
  } else if (parameter.type == PARAMETER_TYPE_SLIDER) {
    Parameter_AddValue(&parameter, encoder_rotation_payload.delta);
    return TASK_STATUS_OK;
  }
  return TASK_STATUS_ERROR;
}

TaskStatus TryUpdateParameterFromAdc(StateEvent& state_event) {
  // TODO:
  // ADC 입력과 매핑된 파라미터를 수정하기
  return TASK_STATUS_ERROR;
}

static TaskStatus TryTransitionUiStateMachine(StateEvent& state_event) {
  // TODO:
  // 좌우 버튼이 페이지 이동에만 사용되므로, 여기서 일어나는 전이들은 대부분
  // System, Loop, IFX A/B/C, TFX A/B/C와 같이 특정 메뉴로 바로 이동하는 전역
  // 버튼으로 발생하거나 Exit 버튼과 같이 상위 메뉴로 이동하는 버튼으로
  // 발생한다. 그러므로, 버튼에 따라서 패널 전이를 하거나 패널 내 페이지 이동을
  // 하도록 요청하면 된다. 패널 전이도 사실상 전역 이동, 상위 패널로 이동밖에
  // 없으니까 기존에 transition_map 대신 상태 머신에서 전역으로 판단하는게
  // 나을것 같다.
  UiStateMachine::Id next_ui_state_id = UiStateMachine::Id::NONE;

  // 1. 버튼 입력일때에만 패널이 바뀜
  if (state_event.type != STATE_EVENT_BUTTON) {
    return TASK_STATUS_ERROR;
  }
  ButtonId button_id;
  if (!ConvertEnumRawToEnum<ButtonId>(state_event.payload.button.button_id_raw,
                                      &button_id)) {
    return TASK_STATUS_ERROR;
  }
  // 2. 버튼 입력은 무조건 PRESSED 상태일 때에만 처리
  ButtonState button_state;
  if (!ConvertEnumRawToEnum<ButtonState>(
          state_event.payload.button.button_state_raw, &button_state)) {
    return TASK_STATUS_ERROR;
  }
  if (button_state != ButtonState::PRESSED) {
    return TASK_STATUS_ERROR;
  }

  if (button_id == ButtonId::EXIT) {
    // 3. exit 버튼이라면 상위 패널로 이동 가능한지 판단 후 전이
    next_ui_state_id = UiStateNavigationTree_GetParent(
        ui_state_machine.GetCurrentState()->GetId());
  } else if (button_id == ButtonId::LEFT) {
    // 4. 좌우 버튼인 경우 페이지 증가
    ui_state_machine.GetCurrentState()->IncreasePageIndex();
  } else if (button_id == ButtonId::RIGHT) {
    ui_state_machine.GetCurrentState()->DecreasePageIndex();
  } else if (button_id == ButtonId::ENCODER_A_PUSH) {
    // 5. 엔코더 푸시 버튼이라면 현재 UiState가 보여주는 슬롯에 따라 전이
    const UiStateMachine::PanelSlot& panel_slot =
        ui_state_machine.GetCurrentState()
            ->GetCurrentPage()[UI_STATE_SLOT_INDEX_A];
    if (panel_slot.type == PANEL_SLOT_TYPE_MENU) {
      next_ui_state_id = panel_slot.data.menu.state_id;
    }
  } else if (button_id == ButtonId::ENCODER_B_PUSH) {
  } else if (button_id == ButtonId::ENCODER_C_PUSH) {
  } else if (button_id == ButtonId::ENCODER_D_PUSH) {
  } else {
    // 6. 전역 버튼이라면 전역 패널 전이 테이블에 따라 전이
    next_ui_state_id = GlobalUiTransitionConfigTable_Get(button_id);
  }
  if (next_ui_state_id != UiStateMachine::Id::NONE) {
    ui_state_machine.TryTransition(next_ui_state_id);
  }

  return TASK_STATUS_OK;
}

static TaskStatus UpdateDisplaySnapshotMailbox() {
  DisplaySnapshot snapshot;

  snapshot.panel.ui_state_enum_raw =
      ConvertEnumToRaw(ui_state_machine.GetCurrentState()->GetId());
  snapshot.panel.page_navigation_flag = PAGE_NAVIGATION_FLAG_NONE;
  if (ui_state_machine.GetCurrentState()->CanDecreasePageIndex()) {
    snapshot.panel.page_navigation_flag |= PAGE_NAVIGATION_FLAG_LEFT_ARROW;
  }
  if (ui_state_machine.GetCurrentState()->CanIncreasePageIndex()) {
    snapshot.panel.page_navigation_flag |= PAGE_NAVIGATION_FLAG_RIGHT_ARROW;
  }

  const UiStateMachine::Page& page =
      ui_state_machine.GetCurrentState()->GetCurrentPage();
  for (size_t i = 0; i < UI_STATE_SLOT_INDEX_COUNT; i++) {
    const UiStateMachine::PanelSlot& panel_slot = page[i];
    snapshot.panel.slot_render_payloads[i].type = page[i].type;
    if (panel_slot.type == PANEL_SLOT_TYPE_MENU) {
      snapshot.panel.slot_render_payloads[i].data.menu = (MenuRenderPayload){
          .menu_icon_encoding_raw16 = ConvertEnumToRaw(panel_slot.data.menu.icon_encoding),
          .label = panel_slot.data.menu.label,
      };
    } else if (panel_slot.type == PANEL_SLOT_TYPE_PARAMETER) {
      Parameter parameter =
          LoopstationStore::GetParameter(panel_slot.data.parameter.id);
      snapshot.panel.slot_render_payloads[i].data.parameter =
          (ParameterRenderPayload){
              .parameter = parameter,
              .label = panel_slot.data.parameter.label,
          };
    }
  }
  snapshot.led = (LedRenderPayload){
      .ifx_a_state = LoopstationStore::GetParameter(ParameterId::IFX_A_STATE),
      .tfx_a_state = LoopstationStore::GetParameter(ParameterId::TFX_A_STATE),
  };
  for (uint8_t i = 0; i < TRACK_COUNT; i++) {
    snapshot.led.track_state_enum_raws[i] = ConvertEnumToRaw(
        track_state_machines[i].GetCurrentState()->GetId());
  }

  xQueueOverwrite((QueueHandle_t)display_snapshot_mailbox, &snapshot);
  return TASK_STATUS_OK;
}

TaskStatus TryTransitionTrackStateMachine(
    TrackStateMachine::StateMachine& track_state_machine,
    StateEvent& state_event) {
  // 1. 버튼 입력일때에만 트랙 상태를 바꿈
  if (state_event.type != STATE_EVENT_BUTTON) {
    return TASK_STATUS_ERROR;
  }
  ButtonPayload& button_payload = state_event.payload.button;
  ButtonId id;
  if (!ConvertEnumRawToEnum<ButtonId>(button_payload.button_id_raw, &id)) {
    return TASK_STATUS_ERROR;
  }
  // 2. 버튼 입력은 무조건 PRESSED 상태일 때에만 처리
  ButtonState state;
  if (!ConvertEnumRawToEnum<ButtonState>(button_payload.button_state_raw,
                                         &state)) {
    return TASK_STATUS_ERROR;
  }
  if (state != ButtonState::PRESSED) {
    return TASK_STATUS_ERROR;
  }

  // 3. 버튼에 매핑된 전이 이벤트가 있는지 확인 후 전이
  TrackStateMachine::ActionId action_id = ButtonToTrackActionMap::Get(id);
  if (action_id == TrackStateMachine::ActionId::NONE) {
    return TASK_STATUS_ERROR;
  }
  track_state_machine.TryTransition(action_id);

  return TASK_STATUS_OK;
}
