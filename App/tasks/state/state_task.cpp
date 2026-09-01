#include "state_task.h"

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "app.h"
#include "display_messages.h"
#include "loopstation_parameter_store.h"
#include "state_messages.h"
#include "state_initparams.h"
#include "ui_state_machine.h"
#include "ui_state_config_table.h"
#include "global_ui_transition_config_table.h"
#include "track_state_machine.h"
#include "button_track_action_config_table.h"
#include "system_state_machine.h"
#include "encoder_id.h"
#include "ui_state_navigation_tree.h"

static StateTaskContext state_task_context;

static osMessageQueueId_t state_event_queue = 0;
static osMessageQueueId_t display_snapshot_mailbox = 0;

static SystemStateMachine system_state_machine;
static SystemStateMachineContext system_state_machine_context;

static UiStateMachine ui_state_machine;
static UiStateMachineContext ui_state_machine_context;

static TrackStateMachine track_state_machine[TRACK_COUNT];
static TrackStateMachineContext track_state_machine_context[TRACK_COUNT];

static void InitStateMachines();
static TaskStatus TryUpdateParameter(StateEvent *state_event);
static TaskStatus TryUpdateParameterFromButton(ButtonPayload *button_payload);
static TaskStatus
TryUpdateParameterFromEncoderRotation(EncoderRotationPayload *encoder_rotation_payload);
static TaskStatus TryUpdateParameterFromAdc(StateEvent *state_event);
static TaskStatus TryTransitionUiStateMachine(UiStateMachine *ui_state_machine,
                                              StateEvent *state_event);
static TaskStatus UpdateDisplaySnapshotMailbox(UiStateMachine *ui_state_machine);
static TaskStatus TryTransitionTrackStateMachine(TrackStateMachine *track_state_machine,
                                                 StateEvent *state_event, uint8_t track_index);

static int IsValidInitParams(const StateInitParams *params)
{
    return (params != 0) && (params->state_event_queue != 0) &&
           (params->display_snapshot_mailbox != 0);
}

void StateTask_Init(void *argument)
{
    const StateInitParams *params = (const StateInitParams *)argument;

    if (!IsValidInitParams(params)) {
        for (;;) {
            osDelay(1);
        }
    }

    state_event_queue = params->state_event_queue;
    display_snapshot_mailbox = params->display_snapshot_mailbox;

    UiStateMachineContext_Init(&ui_state_machine_context);
    for (uint8_t i = 0; i < TRACK_COUNT; i++) {
        TrackStateMachineContext_Init(&track_state_machine_context[i]);
    }

    StateTask_Run();
}

void StateTask_Run(void)
{
    StateEvent state_event;
    osStatus_t os_status;
    TaskStatus task_status;

    InitStateMachines();

    for (;;) {
        if (system_state_machine.current_state->id == SYSTEM_STATE_ID_ERROR) {
            // TODO:
            // 시스템 검증 결과에 오류가 있으면 이를 사용자에게 알려야 함.
            // 지금은 임시로 그냥 무한루프 처리를 했음
            for (;;) {
                osDelay(1);
            }
        }
        os_status = osMessageQueueGet(state_event_queue, &state_event, NULL, osWaitForever);
        if (os_status == osOK) {
            TryUpdateParameter(&state_event);
            TryTransitionUiStateMachine(&ui_state_machine, &state_event);
            for (uint8_t i = 0; i < TRACK_COUNT; i++) {
                TryTransitionTrackStateMachine(&track_state_machine[i], &state_event, i);
            }
            if (task_status != TASK_STATUS_OK) {
                // TODO:
                // 처리 실패에 대한 예외처리 구현하기
            }
            UpdateDisplaySnapshotMailbox(&ui_state_machine);
        }
    }
}

static void InitStateMachines()
{
    SystemStateMachine_Init(&system_state_machine, &system_state_machine_context,
                            SYSTEM_STATE_ID_NOT_INITED);

    UiStateMachine_Init(&ui_state_machine, &ui_state_machine_context,
                        UiStateConfigTable_Get(UI_STATE_ID_HOME));
    for (uint8_t i = 0; i < TRACK_COUNT; i++) {
        TrackStateMachine_Init(&track_state_machine[i], &track_state_machine_context[i],
                               TRACK_STATE_ID_IDLE);
    }
    UpdateDisplaySnapshotMailbox(&ui_state_machine);
}

static TaskStatus TryUpdateParameter(StateEvent *state_event)
{
    if (state_event->type == STATE_EVENT_BUTTON) {
        return TryUpdateParameterFromButton(&state_event->payload.button);
    } else if (state_event->type == STATE_EVENT_ENCODER_ROTATION) {
        return TryUpdateParameterFromEncoderRotation(&state_event->payload.encoder_rotation);
    } else {
        // TODO:
        // ADC 입력에 대한 파라미터 값 변경 기능 구현하기
        return TryUpdateParameterFromAdc(state_event);
    }
}

/**
 * 버튼 입력은 IFX/TFX 토글, 엔코더 버튼만 파라미터 값을 변경한다.
 * */
static TaskStatus TryUpdateParameterFromButton(ButtonPayload *button_payload)
{
    ParameterId parameter_id;
    Parameter *parameter;
    PanelSlot *panel_slot;

    if (button_payload->state != BUTTON_STATE_PRESSED) {
        return TASK_STATUS_ERROR;
    }

    if (button_payload->id != BUTTON_ID_IFX_A_TOGGLE &&
        button_payload->id != BUTTON_ID_TFX_A_TOGGLE &&
        button_payload->id != BUTTON_ID_ENCODER_A_PUSH) {
        return TASK_STATUS_ERROR;
    }

    if (button_payload->id == BUTTON_ID_ENCODER_A_PUSH) {
        // TODO:
        // Encoder_A~D 모두 처리 가능하게 해야함
        panel_slot = UiState_GetPanelSlot(ui_state_machine.current_state, UI_STATE_SLOT_INDEX_A);
        parameter_id = panel_slot->data.parameter.id;
        if (parameter_id == PARAMETER_ID_NONE) {
            return TASK_STATUS_ERROR;
        }
        parameter = LoopStationParameterStore_Get(parameter_id);
        if (parameter->type == PARAMETER_TYPE_TOGGLE) {
            Parameter_ToggleValue(parameter);
            return TASK_STATUS_OK;
        }
    } else if (button_payload->id == BUTTON_ID_IFX_A_TOGGLE) {
        parameter = LoopStationParameterStore_Get(PARAMETER_ID_IFX_A_STATE);
        if (parameter != NULL) {
            Parameter_ToggleValue(parameter);
            return TASK_STATUS_OK;
        }
    } else if (button_payload->id == BUTTON_ID_TFX_A_TOGGLE) {
        parameter = LoopStationParameterStore_Get(PARAMETER_ID_TFX_A_STATE);
        if (parameter != NULL) {
            Parameter_ToggleValue(parameter);
            return TASK_STATUS_OK;
        }
    }

    return TASK_STATUS_ERROR;
}

TaskStatus TryUpdateParameterFromEncoderRotation(EncoderRotationPayload *encoder_rotation_payload)
{
    ParameterId parameter_id;
    Parameter *parameter;
    PanelSlot *slot;
    EncoderId encoder_id;

    encoder_id = encoder_rotation_payload->encoder_id;
    slot = UiState_GetPanelSlot(ui_state_machine.current_state, (UiStateSlotIndex)encoder_id);
    if (slot == NULL || slot->type == PANEL_SLOT_TYPE_MENU) {
        return TASK_STATUS_OK;
    }

    parameter_id = slot->data.parameter.id;
    if (parameter_id == PARAMETER_ID_NONE) {
        return TASK_STATUS_ERROR;
    }
    parameter = LoopStationParameterStore_Get(parameter_id);
    if (parameter->type == PARAMETER_TYPE_TOGGLE) {
        Parameter_ToggleValue(parameter);
        return TASK_STATUS_OK;
    } else if (parameter->type == PARAMETER_TYPE_SLIDER) {
        Parameter_AddValue(parameter, encoder_rotation_payload->delta);
        return TASK_STATUS_OK;
    }
    return TASK_STATUS_ERROR;
}

TaskStatus TryUpdateParameterFromAdc(StateEvent *state_event)
{
    // TODO:
    // ADC 입력과 매핑된 파라미터를 수정하기
    return TASK_STATUS_ERROR;
}

static TaskStatus TryTransitionUiStateMachine(UiStateMachine *ui_state_machine,
                                              StateEvent *state_event)
{
    // TODO:
    // 좌우 버튼이 페이지 이동에만 사용되므로, 여기서 일어나는 전이들은 대부분 System, Loop, IFX
    // A/B/C, TFX A/B/C와 같이 특정 메뉴로 바로 이동하는 전역 버튼으로 발생하거나 Exit 버튼과 같이
    // 상위 메뉴로 이동하는 버튼으로 발생한다.
    // 그러므로, 버튼에 따라서 패널 전이를 하거나 패널 내 페이지 이동을 하도록 요청하면 된다.
    // 패널 전이도 사실상 전역 이동, 상위 패널로 이동밖에 없으니까 기존에 transition_map 대신 상태
    // 머신에서 전역으로 판단하는게 나을것 같다.
    ButtonId button_id;
    UiStateId next_ui_state_id = UI_STATE_ID_NONE;
    PanelSlot *panel_slot;

    // 1. 버튼 입력일때에만 패널이 바뀜
    if (state_event->type != STATE_EVENT_BUTTON) {
        return TASK_STATUS_ERROR;
    }
    button_id = state_event->payload.button.id;
    // 2. 버튼 입력은 무조건 PRESSED 상태일 때에만 처리
    if (state_event->payload.button.state != BUTTON_STATE_PRESSED) {
        return TASK_STATUS_ERROR;
    }

    if (button_id == BUTTON_ID_EXIT) {
        // 3. exit 버튼이라면 상위 패널로 이동 가능한지 판단 후 전이
        next_ui_state_id =
            UiStateNavigationTree_GetParent(ui_state_machine->current_state->ui_state_id);
    } else if (button_id == BUTTON_ID_LEFT) {
        // 4. 좌우 버튼인 경우 페이지 증가
        UiState_IncreasePageIndex(ui_state_machine->current_state);
    } else if (button_id == BUTTON_ID_RIGHT) {
        UiState_DecreasePageIndex(ui_state_machine->current_state);
    } else if (button_id == BUTTON_ID_ENCODER_A_PUSH) {
        // 5. 엔코더 푸시 버튼이라면 현재 UiState가 보여주는 슬롯에 따라 전이
        panel_slot = UiState_GetPanelSlot(ui_state_machine->current_state, UI_STATE_SLOT_INDEX_A);
        if (panel_slot->type == PANEL_SLOT_TYPE_MENU) {
            next_ui_state_id = panel_slot->data.menu.state_id;
        }
    } else if (button_id == BUTTON_ID_ENCODER_B_PUSH) {
    } else if (button_id == BUTTON_ID_ENCODER_C_PUSH) {
    } else if (button_id == BUTTON_ID_ENCODER_D_PUSH) {
    } else {
        // 6. 전역 버튼이라면 전역 패널 전이 테이블에 따라 전이
        next_ui_state_id = GlobalUiTransitionConfigTable_Get(button_id);
    }
    if (next_ui_state_id != UI_STATE_ID_NONE) {
        UiStateMachine_TryTransition(ui_state_machine, next_ui_state_id);
    }

    return TASK_STATUS_OK;
}

TaskStatus UpdateDisplaySnapshotMailbox(UiStateMachine *ui_state_machine)
{
    PanelSlot *slot;
    DisplaySnapshot snapshot;

    snapshot.panel.ui_state_id = ui_state_machine->current_state->ui_state_id;
    snapshot.panel.page_navigation_flag = PAGE_NAVIGATION_FLAG_NONE;
    if (UiState_CanDecreasePageIndex(ui_state_machine->current_state)) {
        snapshot.panel.page_navigation_flag |= PAGE_NAVIGATION_FLAG_LEFT_ARROW;
    }
    if (UiState_CanIncreasePageIndex(ui_state_machine->current_state)) {
        snapshot.panel.page_navigation_flag |= PAGE_NAVIGATION_FLAG_RIGHT_ARROW;
    }
    for (size_t i = 0; i < UI_STATE_SLOT_INDEX_COUNT; i++) {
        slot = UiState_GetPanelSlot(ui_state_machine->current_state, (UiStateSlotIndex)i);

        snapshot.panel.slot_render_payloads[i].type = slot->type;
        if (slot->type == PANEL_SLOT_TYPE_MENU) {
            snapshot.panel.slot_render_payloads[i].data.menu = (MenuRenderPayload){
                .icon_id = slot->data.menu.icon_id,
                .label = slot->data.menu.label,
            };
        } else if (slot->type == PANEL_SLOT_TYPE_PARAMETER) {
            Parameter *parameter = LoopStationParameterStore_Get(slot->data.parameter.id);
            snapshot.panel.slot_render_payloads[i].data.parameter = (ParameterRenderPayload){
                .parameter = *parameter,
                .label = slot->data.parameter.label,
            };
        }
    }
    snapshot.led = (LedRenderPayload){
        .ifx_a_state = *LoopStationParameterStore_Get(PARAMETER_ID_IFX_A_STATE),
        .tfx_a_state = *LoopStationParameterStore_Get(PARAMETER_ID_TFX_A_STATE),
    };
    for (uint8_t i = 0; i < TRACK_COUNT; i++) {
        snapshot.led.track_state[i] = track_state_machine->current_state->id;
    }

    xQueueOverwrite((QueueHandle_t)display_snapshot_mailbox, &snapshot);
    return TASK_STATUS_OK;
}

TaskStatus TryTransitionTrackStateMachine(TrackStateMachine *state_machine, StateEvent *state_event,
                                          uint8_t track_index)
{

    ButtonPayload *button_payload;
    TrackActionId action_id;

    // 1. 버튼 입력일때에만 트랙 상태를 바꿈
    if (state_event->type != STATE_EVENT_BUTTON) {
        return TASK_STATUS_ERROR;
    }
    button_payload = &state_event->payload.button;

    // 2. 버튼 입력은 무조건 PRESSED 상태일 때에만 처리
    if (button_payload->state != BUTTON_STATE_PRESSED) {
        return TASK_STATUS_ERROR;
    }

    // 3. 버튼에 매핑된 전이 이벤트가 있는지 확인 후 전이
    action_id = ButtonTrackActionConfigMap_Get(button_payload->id);
    if (action_id == TRACK_ACTION_ID_NONE) {
        return TASK_STATUS_ERROR;
    }
    TrackStateMachine_TryTransition(state_machine, action_id);

    return TASK_STATUS_OK;
}
