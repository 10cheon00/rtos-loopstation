#ifndef SYSTEM_STATE_MACHINE_H
#define SYSTEM_STATE_MACHINE_H

#include "system_state.h"
#include "system_state_machine_context.h"

typedef struct {
    SystemState* current_state;
    SystemStateMachineContext *context;
} SystemStateMachine;


void SystemStateMachine_Init(SystemStateMachine* state_machine, SystemStateMachineContext* context, SystemStateId init_state_id);
void SystemStateMachine_TryTransition(SystemStateMachine* state_machine, SystemActionId action_id);
void SystemStateMachine_TriggerOnEnter(SystemStateMachine* state_machine);

#endif
