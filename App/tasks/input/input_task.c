#include "input_task.h"

#include "cmsis_os2.h"

#include "input_initparams.h"
#include "input_messages.h"
#include "mcp23017.h"

static osMessageQueueId_t mcp23017_int_event_queue;
static osMessageQueueId_t state_event_queue;

static void InputTask_HandleMcp23017IntEvent(Mcp23017IntEvent *intEvent);

static int InputTask_IsValidInitParams(const InputInitParams *params)
{
    return (params != 0) && (params->mcp23017_int_event_queue != 0 &&
                             params->state_event_queue != 0 && params->hi2c != NULL);
}

void InputTask_Init(void *argument)
{
    const InputInitParams *params = (const InputInitParams *)argument;

    if (!InputTask_IsValidInitParams(params)) {
        for (;;) {
            osDelay(1);
        }
    }

    mcp23017_int_event_queue = params->mcp23017_int_event_queue;
    state_event_queue = params->state_event_queue;

    Mcp23017Infomation mcp23017Infomations = {
        .address = 0x24,
        .pin_status = 0xFF
    };
    Mcp23017InitParams mcp23017InitParams = {
        .hi2c = params->hi2c,
        .infomations = &mcp23017Infomations,
        .infomation_len = 1
    };
    Mcp23017_Init(&mcp23017InitParams);
    InputTask_Run();
}

void InputTask_Run(void)
{
    osStatus_t status;
    Mcp23017IntEvent intEvent;

    for (;;) {
        status = osMessageQueueGet(mcp23017_int_event_queue, &intEvent, NULL, osWaitForever);
        if (status == osOK) {
            InputTask_HandleMcp23017IntEvent(&intEvent);
        }
    }
}

static void InputTask_HandleMcp23017IntEvent(Mcp23017IntEvent *intEvent) {}
