#ifndef AUDIO_INITPARAMS_H
#define AUDUI_INITPARAMS_H

#include "cmsis_os2.h"
#include "stm32h7xx.h"

typedef struct {
  SAI_HandleTypeDef *hsai;
  osEventFlagsId_t system_init_event;
} AudioInitParams;


#endif
