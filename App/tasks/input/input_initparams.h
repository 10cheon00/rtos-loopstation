#ifndef INPUT_INITPARAMS_H
#define INPUT_INITPARAMS_H

#include "cmsis_os2.h"
#include "stm32h7xx.h"

typedef struct {
    osMessageQueueId_t input_event_queue;
    osMessageQueueId_t state_event_queue;
    I2C_HandleTypeDef* hi2c;
    osMutexId_t i2c1_mutex;
} InputInitParams;

#endif
