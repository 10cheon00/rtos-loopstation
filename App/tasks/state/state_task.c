#include "state_task.h"

#include "cmsis_os2.h"
#include "FreeRTOS.h"

#include "app.h"
#include "display_messages.h"
#include "state_messages.h"
#include "state_initparams.h"
#include "state_machine.h"
#include "ui_state.h"
#include "ui_state_ui_panel_id_mapping.h"
#include "loopstation_parameter_store.h"

#define STATE_TASK_TIMEOUT_500MS_TO_TICK (pdMS_TO_TICKS(500UL))

static StateTaskContext state_task_context;
static LoopStationParameterStore *s_loopstation_parameter_store;

static osMessageQueueId_t state_event_queue = 0;
static osMessageQueueId_t display_command_queue = 0;

static StateMachine ui_state_machine;
static UiStateMachineContext ui_state_machine_context;

static TaskStatus StateTask_HandleStateEvent(StateEvent *state_event);

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

    StateTask_Run();
}

void StateTask_Run(void)
{
    StateEvent state_event;
    StateOnEventResultFlags state_on_event_result_flags;
    osStatus_t os_status;
    TaskStatus task_status;

    StateMachine_Init(&ui_state_machine, &UI_STATE_HOME_PANEL, &ui_state_machine_context);

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
            task_status = StateTask_HandleStateEvent(&state_event);
        }
    }
}

TaskStatus StateTask_HandleStateEvent(StateEvent *state_event)
{
    return TASK_STATUS_OK;
}
