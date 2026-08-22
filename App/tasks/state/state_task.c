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
#include "button_ui_action_config_table.h"
#include "track_state_machine.h"
#include "button_track_action_config_table.h"
#include "system_state_machine.h"
#include "encoder_id.h"

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
static UiActionId GetUiActionIdFromButtonId(ButtonId button_id);
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
    ParameterSlotConfig *slots;
    if (button_payload->state != BUTTON_STATE_PRESSED) {
        return TASK_STATUS_ERROR;
    }

    if (button_payload->id != BUTTON_ID_IFX_A_TOGGLE &&
        button_payload->id != BUTTON_ID_TFX_A_TOGGLE &&
        button_payload->id != BUTTON_ID_ENCODER_A_PUSH) {
        return TASK_STATUS_ERROR;
    }

    slots = UiState_GetParameterSlots(ui_state_machine.current_state);
    if (slots == NULL) {
        return TASK_STATUS_OK;
    }
    if (button_payload->id == BUTTON_ID_ENCODER_A_PUSH) {
        // TODO:
        // Encoder_A~D 모두 처리 가능하게 해야함
        parameter_id = slots[ENCODER_ID_A].id;
        if (parameter_id == PARAMETER_ID_NONE) {
            return TASK_STATUS_ERROR;
        }
        parameter = LoopStationParameterStore_GetParameterFromParameterId(parameter_id);
        if (parameter->type == PARAMETER_TYPE_TOGGLE) {
            Parameter_ToggleValue(parameter);
            return TASK_STATUS_OK;
        }
    } else if (button_payload->id == BUTTON_ID_IFX_A_TOGGLE) {
        parameter = LoopStationParameterStore_GetParameterFromParameterId(PARAMETER_ID_IFX_A_STATE);
        if (parameter != NULL) {
            Parameter_ToggleValue(parameter);
            return TASK_STATUS_OK;
        }
    } else if (button_payload->id == BUTTON_ID_TFX_A_TOGGLE) {
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
    ParameterId parameter_id;
    Parameter *parameter;
    ParameterSlotConfig *slots;
    EncoderId encoder_id;

    encoder_id = encoder_rotation_payload->encoder_id;
    slots = UiState_GetParameterSlots(ui_state_machine.current_state);
    if (slots == NULL) {
        return TASK_STATUS_OK;
    }
    parameter_id = slots[encoder_id].id;
    if (parameter_id == PARAMETER_ID_NONE) {
        return TASK_STATUS_ERROR;
    }
    parameter = LoopStationParameterStore_GetParameterFromParameterId(parameter_id);
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
    ui_action_id = ButtonUiActionConfigMap_Get(button_payload->id);
    if (ui_action_id == UI_ACTION_ID_NONE) {
        return TASK_STATUS_ERROR;
    }
    UiStateMachine_TryTransition(ui_state_machine, ui_action_id);

    return TASK_STATUS_OK;
}

TaskStatus UpdateDisplaySnapshotMailbox(UiStateMachine *ui_state_machine)
{
    TrackStateId track_state_ids[TRACK_COUNT];
    DisplaySnapshot snapshot = {
        .ui_state = {.ui_state_id = ui_state_machine->current_state->ui_state_id}};
    ParameterSlotConfig *slots = UiState_GetParameterSlots(ui_state_machine->current_state);
    if (slots != NULL) {
        for (size_t i = 0; i < UI_STATE_SLOT_INDEX_COUNT; i++) {
            snapshot.ui_state.parameter_slots[i].parameter =
                *LoopStationParameterStore_GetParameterFromParameterId(slots[i].id);
            snapshot.ui_state.parameter_slots[i].label = slots[i].label;
        }
    }
    snapshot.led = (LedRenderPayload){
        .ifx_a_state =
            *LoopStationParameterStore_GetParameterFromParameterId(PARAMETER_ID_IFX_A_STATE),
        .tfx_a_state =
            *LoopStationParameterStore_GetParameterFromParameterId(PARAMETER_ID_TFX_A_STATE),
    };
    for (uint8_t i = 0; i < TRACK_COUNT; i++) {
        snapshot.led.track_state[i] = track_state_machine->current_state->id;
    }

    xQueueOverwrite(display_snapshot_mailbox, &snapshot);
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
