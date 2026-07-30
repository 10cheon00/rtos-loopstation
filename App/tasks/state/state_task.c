#include "state_task.h"

#include "cmsis_os2.h"
#include "FreeRTOS.h"

#include "app.h"
#include "display_messages.h"
#include "loopstation_parameter_store.h"
#include "state_messages.h"
#include "state_initparams.h"
#include "ui_state_machine.h"
#include "ui_panel_ui_state_table.h"
#include "button_ui_action_map.h"

typedef enum {
    PARAMETER_UPDATE_RESULT_ERROR = 0,
    PARAMETER_UPDATE_RESULT_OK,
} ParameterUpdateResult;

typedef enum {
    UI_STATE_MACHINE_TRANSITION_RESULT_ERROR = 0,
    UI_STATE_MACHINE_TRANSITION_RESULT_OK,
} UiStateMachineTransitionResult;

static StateTaskContext state_task_context;

static osMessageQueueId_t state_event_queue = 0;
static osMessageQueueId_t display_command_queue = 0;

static UiStateMachine ui_state_machine;
static UiStateMachineContext ui_state_machine_context;

static ParameterUpdateResult TryUpdateParameter(StateEvent *state_event);
static ParameterUpdateResult
TryUpdateParameterFromControlButton(ControlButtonPayload *control_button_payload);
static ParameterUpdateResult
TryUpdateParameterFromEncoderRotation(EncoderRotationPayload *encoder_rotation_payload);
static ParameterUpdateResult TryUpdateParameterFromAdc(StateEvent *state_event);
static UiStateMachineTransitionResult TryTransitionUiStateMachine(UiStateMachine *ui_state_machine,
                                                                  StateEvent *state_event);
static UiActionId GetUiActionIdFromControlButtonId(ControlButtonId control_button_id);
static TaskStatus
TryRenderCurrentUiState(UiStateMachine *ui_state_machine,
                        ParameterUpdateResult parameter_update_result,
                        UiStateMachineTransitionResult ui_state_machine_transition_result);

static int IsValidInitParams(const StateInitParams *params)
{
    return (params != 0) && (params->state_event_queue != 0) &&
           (params->display_command_queue != 0);
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
    display_command_queue = params->display_command_queue;

    UiStateMachineContext_Init(&ui_state_machine_context, display_command_queue);

    StateTask_Run();
}

void StateTask_Run(void)
{
    StateEvent state_event;
    osStatus_t os_status;
    TaskStatus task_status;
    ParameterUpdateResult parameter_update_result;
    UiStateMachineTransitionResult ui_state_machine_transition_result;

    UiStateMachine_Init(&ui_state_machine, &ui_state_machine_context,
                        UiPanelUiStateTable_GetUiStateFromUiPanelId(UI_PANEL_ID_HOME));

    for (;;) {
        os_status = osMessageQueueGet(state_event_queue, &state_event, NULL, osWaitForever);
        if (os_status == osOK) {
            /**
             * 외부 입력이 발생했을 때
             * 1. 해당 입력이 패널과 관계없이 파라미터를 수정할 수 있다면 수정한다.
             * 2. 해당 입력이 현재 패널 상태에 따라 파라미터를 수정할 수 있다면 수정한다.
             * 3. 해당 입력이 트랙 상태 머신의 상태를 바꾼다면 트랙 상태 머신에게 알린다.
             * 4. 해당 입력으로 인해 파라미터가 수정되었다면 ui 상태 머신에게 알린다.
             * 5. 해당 입력이 현재 패널을 다른 패널로 전이시킨다면 ui 상태 머신에게 알린다.
             * 6. 현재 패널 상태에 따라 트랙 상태가 전이된 것을 ui 상태 머신에게 알려야 하는 경우
             * 알린다.
             *
             * ui 상태머신이 4,5,6의 조건을 검사해야하나? 아니지 않나...
             * 3 역시 트랙 상태 머신이 판단해야하나?
             * 그러면 상태 머신에게 이벤트 처리를 위임하는게 아니라, 이벤트 처리는 StateTask에서
             * 하고, 그 결과를 상태 머신에게 위임한다.
             * 트랙 상태 머신은 상태에 따라 OnEnter에서 오디오 처리 태스크에게 요청하기.
             * UI 상태 머신은 OnEnter에서 디스플레이 태스크에게 요청하기, OnEvent에서 디스플레이
             * 태스크에게 요청하기
             */
            parameter_update_result = TryUpdateParameter(&state_event);
            ui_state_machine_transition_result =
                TryTransitionUiStateMachine(&ui_state_machine, &state_event);
            task_status = TryRenderCurrentUiState(&ui_state_machine, parameter_update_result,
                                                  ui_state_machine_transition_result);
            if (task_status != TASK_STATUS_OK) {
            }
        }
    }
}

static ParameterUpdateResult TryUpdateParameter(StateEvent *state_event)
{
    if (state_event->type == STATE_EVENT_CONTROL_BUTTON) {
        return TryUpdateParameterFromControlButton(&state_event->payload.control_button);
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
static ParameterUpdateResult
TryUpdateParameterFromControlButton(ControlButtonPayload *control_button_payload)
{
    UiPanelId ui_panel_id;
    ParameterId parameter_id;
    Parameter *parameter;

    if (control_button_payload->id != CONTROL_BUTTON_ID_IFX_A_TOGGLE &&
        control_button_payload->id != CONTROL_BUTTON_ID_TFX_A_TOGGLE &&
        control_button_payload->id != CONTROL_BUTTON_ID_ENCODER_A_PUSH) {
        return TASK_STATUS_ERROR;
    }

    if (control_button_payload->id == CONTROL_BUTTON_ID_ENCODER_A_PUSH) {
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
    } else {
        // TODO:
        // IFX, TFX 파라미터를 추가해야 함
    }

    return TASK_STATUS_ERROR;
}

ParameterUpdateResult
TryUpdateParameterFromEncoderRotation(EncoderRotationPayload *encoder_rotation_payload)
{
    UiPanelId ui_panel_id;
    ParameterId parameter_id;
    Parameter *parameter;
    uint8_t encoder_id;
    uint8_t delta = 1, scale = 1;

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
        if (state_task_context.encoder_button_state_snapshot == CONTROL_BUTTON_STATE_PRESSED) {
            scale = 10;
        }
        delta = encoder_rotation_payload->delta;
        Parameter_AddValue(parameter, delta * scale);
        return TASK_STATUS_OK;
    }
    return TASK_STATUS_ERROR;
}

ParameterUpdateResult TryUpdateParameterFromAdc(StateEvent *state_event)
{
    // TODO:
    // ADC 입력과 매핑된 파라미터를 수정하되, 현재 패널에 그 파라미터가 렌더링 되어야 하는 경우
    // 판단하기
    return TASK_STATUS_ERROR;
}

static UiStateMachineTransitionResult TryTransitionUiStateMachine(UiStateMachine *ui_state_machine,
                                                                  StateEvent *state_event)
{
    ControlButtonPayload *control_button_payload;
    UiActionId ui_action_id;

    // 1. 버튼 입력일때에만 패널이 바뀜
    if (state_event->type != STATE_EVENT_CONTROL_BUTTON) {
        return UI_STATE_MACHINE_TRANSITION_RESULT_ERROR;
    }
    control_button_payload = &state_event->payload.control_button;

    // 2. 버튼 입력은 무조건 PRESSED 상태일 때에만 처리
    if (control_button_payload->state != CONTROL_BUTTON_STATE_PRESSED) {
        return UI_STATE_MACHINE_TRANSITION_RESULT_ERROR;
    }

    // 3. 버튼에 매핑된 전이 이벤트가 있는지 확인 후 전이
    ui_action_id = GetUiActionIdFromControlButtonId(control_button_payload->id);
    if (ui_action_id == UI_ACTION_NONE) {
        return UI_STATE_MACHINE_TRANSITION_RESULT_ERROR;
    }
    UiStateMachine_TryTransition(ui_state_machine, ui_action_id);

    return UI_STATE_MACHINE_TRANSITION_RESULT_OK;
}

static UiActionId GetUiActionIdFromControlButtonId(ControlButtonId control_button_id)
{
    for (size_t i = 0; i < button_ui_action_map_count; i++) {
        if (control_button_id == button_ui_action_map[i].button_id) {
            return button_ui_action_map[i].ui_action_id;
        }
    }
    return UI_ACTION_NONE;
}

static TaskStatus
TryRenderCurrentUiState(UiStateMachine *ui_state_machine,
                        ParameterUpdateResult parameter_update_result,
                        UiStateMachineTransitionResult ui_state_machine_transition_result)
{
    if (parameter_update_result == PARAMETER_UPDATE_RESULT_OK ||
        ui_state_machine_transition_result == UI_STATE_MACHINE_TRANSITION_RESULT_OK) {
        // TODO:
        // 상태 전이가 성공했는지 판단하기
        UiStateMachine_RenderCurrentState(ui_state_machine);
        return TASK_STATUS_OK;
    }
    return TASK_STATUS_ERROR;
}
