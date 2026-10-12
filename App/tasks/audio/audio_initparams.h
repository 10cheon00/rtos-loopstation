#ifndef AUDIO_INITPARAMS_H
#define AUDUI_INITPARAMS_H

#include "cmsis_os2.h"
#include "stm32h7xx.h"

typedef struct {
  SAI_HandleTypeDef* hsai_tx;
  SAI_HandleTypeDef* hsai_rx;
  osEventFlagsId_t system_init_event;
  osMessageQueueId_t audio_event_snapshot_mailbox;
  osMessageQueueId_t audio_dma_event_queue;
  osMutexId_t loopstation_parameter_mutex;
  osMutexId_t fx_parameter_mutex;
  SDRAM_HandleTypeDef* hsdram;
} AudioInitParams;

#endif
