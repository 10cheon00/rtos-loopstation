#ifndef INPUT_TASK_H
#define INPUT_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "app.h"

typedef struct {
    uint32_t previous_encoder_counter;
} InputTaskContext;

void InputTask_Init(void *argument);
void InputTask_Run(void);

#ifdef __cplusplus
}
#endif

#endif
