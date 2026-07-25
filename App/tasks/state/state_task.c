#include "state_task.h"

#include "cmsis_os2.h"
#include "FreeRTOS.h"

#include "app.h"
#include "display_messages.h"
#include "state_messages.h"
#include "state_initparams.h"
#include "state_machine.h"
#include "ui_state_home_panel.h"
#include "ui_state_setting_panel.h"
#include "ui_state_context.h"
#include "loopstation_parameter_store.h"

#define STATE_TASK_TIMEOUT_500MS_TO_TICK (pdMS_TO_TICKS(500UL))

static StateTaskContext state_task_context;
static LoopStationParameterStore *s_loopstation_parameter_store;

static osMessageQueueId_t state_event_queue = 0;
static osMessageQueueId_t display_command_queue = 0;

static RendererContext renderer_context;
static StateMachine ui_state_machine;

static TaskStatus StateTask_HandleStateEvent(const StateEvent *state_event,
                                             StateOnEventResultFlags *state_on_event_result_flags);
static TaskStatus StateTask_GetUiStateByUiPanelId(const UiPanelId ui_panel_id, State* next_state);
static TaskStatus StateTask_ModifyParameters(const StateEvent *state_event,
                                             StateOnEventResultFlags *state_on_event_result_flags);
static TaskStatus
StateTask_ModifyParametersByEncoder(const StateEvent *state_event, uint8_t parameter_index,
                                    StateOnEventResultFlags *state_on_event_result_flags);
static UiPanelParameterBinding *StateTask_GetCurrentUiPanelParameters();
static TaskStatus StateTask_RequestRendering(StateOnEventResultFlags *state_on_event_result_flags);

static int StateTask_IsValidInitParams(const StateInitParams *params)
{
    return (params != 0) && (params->state_event_queue != 0) &&
           (params->display_command_queue != 0);
}

void StateTask_Init(void *argument)
{
    const StateInitParams *params = (const StateInitParams *)argument;

    if (!StateTask_IsValidInitParams(params)) {
        for (;;) {
            osDelay(1);
        }
    }

    s_loopstation_parameter_store = LoopstationParameterStore_GetInstance();

    state_event_queue = params->state_event_queue;
    display_command_queue = params->display_command_queue;

    renderer_context.display_command_queue = display_command_queue;
    ui_state_machine.context = &(RendererContext){.display_command_queue = display_command_queue};
    ui_state_machine.current_state = (State *)&UI_STATE_HOME_PANEL;

    StateTask_Run();
}

void StateTask_Run(void)
{
    StateEvent state_event;
    StateOnEventResultFlags state_on_event_result_flags;
    osStatus_t os_status;
    TaskStatus task_status;

    ui_state_machine.current_state->on_enter(NULL);

    for (;;) {
        os_status = osMessageQueueGet(state_event_queue, &state_event, NULL, osWaitForever);
        if (os_status == osOK) {
            // 상태 전이는 switch case로 작성하지 않는다.
            // 각 상태 머신마다 폴더를 갖고, 폴더에 나열된 파일들이 각 상태를 나타낸다.
            // 각 상태는 정해진 구조체에 따라 객체처럼 공통된 API를 구현한다.
            // 외부에서 전해진 이벤트를 상태에 전달함으로 이벤트 처리를 위임힌다.
            task_status = StateTask_HandleStateEvent(&state_event, &state_on_event_result_flags);
            if (task_status != TASK_STATUS_OK) {
                // TODO:
                // 이벤트 처리에 실패하는 경우에 대해 처리하기
            }
            if ((state_on_event_result_flags & (STATE_ON_EVENT_HANDLING_FLAG_PARAMETER_UPDATED |
                                                STATE_ON_EVENT_HANDLING_FLAG_TRANSITION)) != 0) {
                task_status = StateTask_RequestRendering(&state_on_event_result_flags);
                if (task_status == TASK_STATUS_ERROR) {
                    // TODO:
                    // 렌더링 요청에 실패하는 경우에 대해 처리하기
                }
            }
        }
    }
}

static TaskStatus StateTask_HandleStateEvent(const StateEvent *state_event,
                                             StateOnEventResultFlags *state_on_event_result_flags)
{
    TaskStatus task_status;
    UiPanelId ui_panel_id;
    State next_state;

    switch (state_event->type) {
    case STATE_EVENT_CONTROL_BUTTON:
        // TODO:
        // 버튼 스냅샷을 업데이트하는 코드를 추가해야 한다.
        // 그리고 코드가 너무 길어보이는데 리팩토링하기

        // 좌우 버튼은 무조건 패널 전환에 쓰이므로 바로 UI 상태 머신에 전달한다.
        // TODO:
        // 좌우 버튼이 아닌 다른 버튼을 눌렀을 때는 파라미터 값 변경이 일어나지 않는가?
        if (state_event->payload.control_button.state == CONTROL_BUTTON_STATE_RELEASED) {
            *state_on_event_result_flags =
                ui_state_machine.current_state->on_event(state_event, &ui_panel_id);
            if ((*state_on_event_result_flags & STATE_ON_EVENT_HANDLING_FLAG_TRANSITION) != 0) {
                task_status = StateTask_GetUiStateByUiPanelId(ui_panel_id, &next_state);
                if (task_status != TASK_STATUS_ERROR) {
                    // TODO:
                    // UiStateMachine에서 renderer_context를 제거하기
                    StateTransition transition = {.cause_event = state_event,
                                                  .to = &next_state,
                                                  .context = &renderer_context};
                    StateMachine_DoTransition(&ui_state_machine, &transition);
                }

                return TASK_STATUS_OK;
            }
        }
        break;
    case STATE_EVENT_ENCODER_ROTATION:
        // 여기서는 UI 상태를 전이시키지 않고, 파라미터 값을 바꾼다.
        // 예를 들어 설정 패널에서는 커서 파라미터 값을 바꾼다.
        return StateTask_ModifyParameters(state_event, state_on_event_result_flags);
    default:
        break;
    }

    return TASK_STATUS_OK;
}

TaskStatus StateTask_GetUiStateByUiPanelId(const UiPanelId ui_panel_id, State* next_state)
{
    TODO:
    // 패널 id에 따른 UiState를 반환하기 위해 매핑 관계를 정의한 파일 작성 필요
    return TASK_STATUS_OK;
}

/** 파라미터를 수정하는 것은 무조건 엔코더 또는 포텐셔미터를 통해 발생하므로,
 * 두 경우를 구분하여 처리한다.
 * - 엔코더로 변경하는 경우
 *  엔코더의 방향 그리고 푸시버튼의 여부에 따라 파라미터 값에 증감을 한다.
 * - 포텐셔미터로 변경하는 경우
 *  ADC 변환값이 그대로 파라미터에 대입된다.
 */
static TaskStatus StateTask_ModifyParameters(const StateEvent *state_event,
                                             StateOnEventResultFlags *state_on_event_result_flags)
{
    // UI 상태 머신의 상태에 따라 정해진 파라미터만 수정한다.
    // 패널에 바인딩된 여러 파라미터 중 엔코더 id를 인덱스로 하여 수정한다.
    if (state_event->type == STATE_EVENT_ENCODER_ROTATION) {
        uint8_t parameter_index = state_event->payload.encoder_rotation.encoder_id;
        return StateTask_ModifyParametersByEncoder(state_event, parameter_index,
                                                   state_on_event_result_flags);
    } else {
        // TODO:
        // 여기서는 ADC 변환값을 파라미터에 대입하도록 한다.
        // ADC 종류에 따라 바인딩된 파라미터만 수정해야한다.
    }

    return TASK_STATUS_ERROR;
}

static TaskStatus
StateTask_ModifyParametersByEncoder(const StateEvent *state_event, uint8_t parameter_index,
                                    StateOnEventResultFlags *state_on_event_result_flags)
{
    UiPanelParameterBinding *binding;
    Parameter_t value, scale = 1;
    ParameterId parameter_store_index;
    Parameter *parameter;

    if (parameter_index < 0 || parameter_index >= 4) {
        return TASK_STATUS_ERROR;
    }

    binding = StateTask_GetCurrentUiPanelParameters();
    if (binding->ui_panel_id == UI_PANEL_ID_NONE) {
        return TASK_STATUS_ERROR;
    }

    if (state_task_context.encoder_button_state_snapshot == CONTROL_BUTTON_STATE_PRESSED) {
        scale = 10;
    }
    value = scale * state_event->payload.encoder_rotation.delta;

    parameter_store_index = binding->parameter_id[parameter_index];
    parameter = &s_loopstation_parameter_store->parameters[parameter_store_index];
    Parameter_AddValue(parameter, value);

    *state_on_event_result_flags = STATE_ON_EVENT_HANDLING_FLAG_PARAMETER_UPDATED;
    return TASK_STATUS_OK;
}

static UiPanelParameterBinding *StateTask_GetCurrentUiPanelParameters()
{
    UiPanelId ui_panel_id = ui_state_machine.current_state->id;

    for (uint16_t i = 0; i < UI_PANEL_PARAMETER_BINDING_COUNT; i++) {
        if (ui_panel_parameter_binding[i].ui_panel_id == ui_panel_id) {
            return (UiPanelParameterBinding *)&ui_panel_parameter_binding[i];
        }
    }

    return (UiPanelParameterBinding *)&ui_panel_parameter_binding[0];
}

static TaskStatus StateTask_RequestRendering(StateOnEventResultFlags *state_on_event_result_flags)
{
    osStatus_t os_status;
    DisplayCommand command = {
        .type = DISPLAY_COMMAND_UI_STATE_RENDER,
        .payload = {.ui_state_render = {.panel_id = ui_state_machine.current_state->id}}};
    os_status =
        osMessageQueuePut(display_command_queue, &command, 0, STATE_TASK_TIMEOUT_500MS_TO_TICK);
    if (os_status != osOK) {
        return TASK_STATUS_ERROR;
    }
    return TASK_STATUS_OK;
}
