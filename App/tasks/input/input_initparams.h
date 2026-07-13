#ifndef INPUT_INITPARAMS_H
#define INPUT_INITPARAMS_H

#include "cmsis_os2.h"

typedef struct {
    osMessageQueueId_t mcp23017_int_event_queue;
    osMessageQueueId_t state_event_queue;
} InputInitParams;

#endif
