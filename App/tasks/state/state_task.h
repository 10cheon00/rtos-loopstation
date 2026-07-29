#ifndef STATE_TASK_H
#define STATE_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "control_button_id.h"

void StateTask_Init(void *argument);
void StateTask_Run(void);

typedef struct {
    ControlButtonState encoder_button_state_snapshot;
} StateTaskContext;

#ifdef __cplusplus
}
#endif

#endif
