#include "system_state_machine.h"

#include "system_state_config_table.h"

void SystemStateMachine_Init(SystemStateMachine* state_machine,
                             SystemStateMachineContext* context,
                             SystemStateId init_state_id) {
  SystemState* next_state;
  state_machine->context = context;
  next_state = SystemStateConfigMap_Get(init_state_id);
  if (next_state != NULL) {
    state_machine->current_state = next_state;
    SystemStateMachine_TriggerOnEnter(state_machine);
  }
}

void SystemStateMachine_TryTransition(SystemStateMachine* state_machine,
                                      SystemActionId action_id) {
  SystemStateId next_state_id;
  SystemState* next_state;
  next_state_id =
      SystemState_GetNextSystemStateId(state_machine->current_state, action_id);
  if (next_state_id == SYSTEM_STATE_ID_NONE) {
    return;
  }
  next_state = SystemStateConfigMap_Get(next_state_id);
  if (next_state != NULL) {
    state_machine->current_state = next_state;
    SystemStateMachine_TriggerOnEnter(state_machine);
  }
}

void SystemStateMachine_TriggerOnEnter(SystemStateMachine* state_machine) {
  SystemStateOnEnterResult result =
      state_machine->current_state->OnEnter(state_machine->context);
  if (result.is_transition_requested) {
    SystemStateMachine_TryTransition(state_machine, result.action_id);
  }
}
