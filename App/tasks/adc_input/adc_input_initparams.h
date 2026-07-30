#ifndef ADC_INPUT_INITPARAMS_H
#define ADC_INPUT_INITPARAMS_H

#include "cmsis_os2.h"
#include "stm32h7xx.h"

typedef struct {
    ADC_HandleTypeDef *hadc;
    osMessageQueueId_t input_message_queue;
} AdcInputInitParams;

#endif
