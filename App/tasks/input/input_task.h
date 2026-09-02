#ifndef INPUT_TASK_H
#define INPUT_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "app.h"
#include "button_id.h"
#include "encoder_id.h"

typedef struct {
  ButtonState encoder_button_state[ENCODER_ID_COUNT];
} InputTaskContext;

void InputTask_Init(void* argument);
void InputTask_Run(void);

#ifdef __cplusplus
}
#endif

#endif
