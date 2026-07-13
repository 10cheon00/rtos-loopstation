#ifndef DISPLAY_INITPARAMS_H
#define DISPLAY_INITPARAMS_H

#include "stm32h7xx.h"
#include "cmsis_os2.h"

typedef struct {
    osMessageQueueId_t display_command_queue;
    SPI_HandleTypeDef *hspi;
} DisplayInitParams;

#endif
