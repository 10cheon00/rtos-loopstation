#ifndef ADC_INPUT_TASK_H
#define ADC_INPUT_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "app.h"

void AdcInputTask_Init(void* arguments);
void AdcInputTask_Run(void);

#ifdef __cplusplus
}
#endif

#endif
