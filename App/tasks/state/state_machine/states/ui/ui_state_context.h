#ifndef DISPLAY_CONTEXT_H
#define DISPLAY_CONTEXT_H

#include "cmsis_os2.h"

typedef struct {
    osMessageQueueId_t display_command_queue;
} RendererContext;

#endif
