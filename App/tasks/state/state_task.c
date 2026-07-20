#include "state_task.h"

#include "cmsis_os2.h"

#include "app.h"
#include "state_messages.h"
#include "state_initparams.h"
#include "state_machine.h"
#include "home_panel.h"
#include "setting_panel.h"
#include "display_context.h"

static osMessageQueueId_t state_event_queue = 0;
static osMessageQueueId_t display_command_queue = 0;

static DisplayPanelContext display_panel_context;
static StateMachine display_state_machine;

static TaskStatus StateTask_DispatchStateEvent(const StateEvent *state_event);

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

    state_event_queue = params->state_event_queue;
    display_command_queue = params->display_command_queue;

    display_panel_context.display_command_queue = display_command_queue;
    display_state_machine.context =
        &(DisplayPanelContext){.display_command_queue = display_command_queue};
    display_state_machine.current_state = &DISPLAY_STATE_HOME_PANEL;

    StateTask_Run();
}

void StateTask_Run(void)
{
    StateEvent state_event;
    TaskStatus task_status;
    osStatus_t os_status;

    display_state_machine.current_state->on_enter(&display_panel_context);

    for (;;) {
        os_status = osMessageQueueGet(state_event_queue, &state_event, NULL, osWaitForever);
        if (os_status == osOK) {
            // 상태 전이는 switch case로 작성하지 않는다.
            // 각 상태 머신마다 폴더를 갖고, 폴더에 나열된 파일들이 각 상태를 나타낸다.
            // 각 상태는 정해진 구조체에 따라 객체처럼 공통된 API를 구현한다.
            // 외부에서 전해진 이벤트를 상태에 전달함으로 이벤트 처리를 위임힌다.
            task_status = StateTask_DispatchStateEvent(&state_event);

            if (task_status != TASK_STATUS_OK) {
            }
        }
    }
}

static TaskStatus StateTask_DispatchStateEvent(const StateEvent *state_event)
{
    EventHandlingResult result;

    switch (state_event->type) {
    case STATE_EVENT_CONTROL_BUTTON:
        if (state_event->payload.control_button.state == CONTROL_BUTTON_STATE_RELEASED) {
            result = display_state_machine.current_state->on_event(state_event);
        }
        break;
    default:
        break;
    }

    if (result.status == EVENT_HANDLING_STATUS_TRANSITION) {
        StateTransition transition = {.cause_event = state_event, .to = result.next_state, .context = &display_panel_context};
        StateMachine_DoTransition(&display_state_machine, &transition);
    }

    return TASK_STATUS_OK;
}
