#include "state_task.h"

#include "cmsis_os2.h"
#include "state_initparams.h"

static int StateTask_IsValidInitParams(const StateInitParams *params)
{
    return (params != 0) &&
           (params->state_event_queue != 0) &&
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

    StateTask_Run();
}

void StateTask_Run(void)
{
    for (;;) {
        osDelay(1);
    }
}
