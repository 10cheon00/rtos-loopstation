#ifndef UI_STATE_H
#define UI_STATE_H

#include "cmsis_os2.h"

typedef struct {
    osMessageQueueId_t display_command_queue;
} UiStateMachineContext;

#endif