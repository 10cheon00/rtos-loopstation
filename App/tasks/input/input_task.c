#include "input_task.h"

#include "input_initparams.h"
#include "cmsis_os2.h"

static int InputTask_IsValidInitParams(const InputInitParams *params)
{
    return (params != 0) &&
           (params->mcp23017_int_event_queue != 0) &&
           (params->state_event_queue != 0);
}

void InputTask_Init(void *argument)
{
    const InputInitParams *params = (const InputInitParams *)argument;

    if (!InputTask_IsValidInitParams(params)) {
        for (;;) {
            osDelay(1);
        }
    }

    InputTask_Run();
}

void InputTask_Run(void)
{
    for (;;) {
        osDelay(1);
    }
}
