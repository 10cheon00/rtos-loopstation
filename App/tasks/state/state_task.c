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
#include "ui_panel_ui_state_table.h"
#include "button_ui_action_map.h"

static StateTaskContext state_task_context;

static osMessageQueueId_t state_event_queue = 0;
static osMessageQueueId_t display_snapshot_mailbox = 0;

static UiStateMachine ui_state_machine;
static UiStateMachineContext ui_state_machine_context;

static TaskStatus TryUpdateParameter(StateEvent *state_event);
static TaskStatus TryUpdateParameterFromButton(ButtonPayload *button_payload);
static TaskStatus
TryUpdateParameterFromEncoderRotation(EncoderRotationPayload *encoder_rotation_payload);
static TaskStatus TryUpdateParameterFromAdc(StateEvent *state_event);
static TaskStatus TryTransitionUiStateMachine(UiStateMachine *ui_state_machine,
                                              StateEvent *state_event);
static UiActionId GetUiActionIdFromButtonId(ButtonId button_id);
static TaskStatus UpdateDisplaySnapshotMailbox(UiStateMachine *ui_state_machine);

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

    StateTask_Run();
}

void StateTask_Run(void)
{
    StateEvent state_event;
    osStatus_t os_status;
    TaskStatus task_status;

    UiStateMachine_Init(&ui_state_machine, &ui_state_machine_context,
                        UiPanelUiStateTable_GetUiStateFromUiPanelId(UI_PANEL_ID_HOME));
    UpdateDisplaySnapshotMailbox(&ui_state_machine);

    for (;;) {
        os_status = osMessageQueueGet(state_event_queue, &state_event, NULL, osWaitForever);
        if (os_status == osOK) {
            TryUpdateParameter(&state_event);
            TryTransitionUiStateMachine(&ui_state_machine, &state_event);

            if (task_status != TASK_STATUS_OK) {
            }
            UpdateDisplaySnapshotMailbox(&ui_state_machine);
        }
    }
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
    UiPanelId ui_panel_id;
    ParameterId parameter_id;
    Parameter *parameter;

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
        ui_panel_id = ui_state_machine.current_state->ui_panel_id;
        parameter_id = PanelParameterTable_GetParameterId(ui_panel_id, 0);
        if (parameter_id == PARAMETER_ID_NONE) {
            return TASK_STATUS_ERROR;
        }
        parameter = LoopStationParameterStore_GetParameterFromParameterId(parameter_id);
        if (parameter->type == PARAMETER_TYPE_TOGGLE) {
            Parameter_ToggleValue(parameter);
            return TASK_STATUS_OK;
        }
    } else if (button_payload->id == BUTTON_ID_IFX_A_TOGGLE){
        parameter = LoopStationParameterStore_GetParameterFromParameterId(PARAMETER_ID_IFX_A_STATE);
        if (parameter != NULL) {
            Parameter_ToggleValue(parameter);
            return TASK_STATUS_OK;
        }
    } else if (button_payload->id == BUTTON_ID_TFX_A_TOGGLE){
        parameter = LoopStationParameterStore_GetParameterFromParameterId(PARAMETER_ID_TFX_A_STATE);
        if (parameter != NULL) {
            Parameter_ToggleValue(parameter);
            return TASK_STATUS_OK;
        }
    }

    return TASK_STATUS_ERROR;
}

TaskStatus TryUpdateParameterFromEncoderRotation(EncoderRotationPayload *encoder_rotation_payload)
{
    UiPanelId ui_panel_id;
    ParameterId parameter_id;
    Parameter *parameter;
    uint8_t encoder_id;
    int8_t delta = 1, scale = 1;

    encoder_id = encoder_rotation_payload->encoder_id;
    ui_panel_id = ui_state_machine.current_state->ui_panel_id;
    parameter_id = PanelParameterTable_GetParameterId(ui_panel_id, encoder_id);
    if (parameter_id == PARAMETER_ID_NONE) {
        return TASK_STATUS_ERROR;
    }
    parameter = LoopStationParameterStore_GetParameterFromParameterId(parameter_id);
    if (parameter->type == PARAMETER_TYPE_TOGGLE) {
        Parameter_ToggleValue(parameter);
        return TASK_STATUS_OK;
    } else if (parameter->type == PARAMETER_TYPE_SLIDER) {
        if (state_task_context.encoder_button_state_snapshot == BUTTON_STATE_PRESSED) {
            scale = 10;
        }
        delta = encoder_rotation_payload->delta;
        Parameter_AddValue(parameter, delta * scale);
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
    ButtonPayload *button_payload;
    UiActionId ui_action_id;

    // 1. 버튼 입력일때에만 패널이 바뀜
    if (state_event->type != STATE_EVENT_BUTTON) {
        return TASK_STATUS_ERROR;
    }
    button_payload = &state_event->payload.button;

    // 2. 버튼 입력은 무조건 PRESSED 상태일 때에만 처리
    if (button_payload->state != BUTTON_STATE_PRESSED) {
        return TASK_STATUS_ERROR;
    }

    // 3. 버튼에 매핑된 전이 이벤트가 있는지 확인 후 전이
    ui_action_id = GetUiActionIdFromButtonId(button_payload->id);
    if (ui_action_id == UI_ACTION_NONE) {
        return TASK_STATUS_ERROR;
    }
    UiStateMachine_TryTransition(ui_state_machine, ui_action_id);

    return TASK_STATUS_OK;
}

static UiActionId GetUiActionIdFromButtonId(ButtonId button_id)
{
    for (size_t i = 0; i < button_ui_action_map_count; i++) {
        if (button_id == button_ui_action_map[i].button_id) {
            return button_ui_action_map[i].ui_action_id;
        }
    }
    return UI_ACTION_NONE;
}

TaskStatus UpdateDisplaySnapshotMailbox(UiStateMachine *ui_state_machine)
{
    ParameterId *parameter_ids =
        PanelParameterTable_GetParameterIds(ui_state_machine->current_state->ui_panel_id);
    Parameter *parameter;

    DisplaySnapshot snapshot = {
        .ui_state = {.panel_id = ui_state_machine->current_state->ui_panel_id}};
    for (size_t i = 0; i < UI_PANEL_MAX_PARAMETER_COUNT; i++) {
        snapshot.ui_state.parameters[i] = *LoopStationParameterStore_GetParameterFromParameterId(parameter_ids[i]);
    }
    // TODO:
    // LED와 관련된 정보도 같이 전송하도록 구현하기
    snapshot.led = (LedRenderPayload){
        .ifx_a_state = *LoopStationParameterStore_GetParameterFromParameterId(PARAMETER_ID_IFX_A_STATE),
        .tfx_a_state = *LoopStationParameterStore_GetParameterFromParameterId(PARAMETER_ID_TFX_A_STATE),
    };
    
    xQueueOverwrite(display_snapshot_mailbox, &snapshot);
    return TASK_STATUS_OK;
}
