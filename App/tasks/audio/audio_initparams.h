#ifndef AUDIO_INITPARAMS_H
#define AUDIO_INITPARAMS_H

#include "cmsis_os2.h"
#include "stm32h7xx.h"

typedef struct {
    osMessageQueueId_t audio_event_queue;
    SAI_HandleTypeDef *hsaiTx;
    SAI_HandleTypeDef *hsaiRx;
} AudioInitParams;

#endif
