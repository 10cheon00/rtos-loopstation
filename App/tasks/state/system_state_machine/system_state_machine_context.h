#ifndef SYSTEM_MACHINE_CONTEXT_H
#define SYSTEM_MACHINE_CONTEXT_H

#include "config_validator.h"

typedef struct {
  ConfigValidatorLog* logs;
} SystemStateMachineContext;

void SystemStateMachineContext_Init(SystemStateMachineContext* context);

#endif
