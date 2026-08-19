#ifndef AUDIO_INITPARAMS_H
#define AUDIO_INITPARAMS_H

#include "cmsis_os2.h"

typedef struct {
    osMessageQueueId_t audio_event_queue;
} AudioInitParams;

#endif
