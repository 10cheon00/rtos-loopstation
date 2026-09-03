#include "system_state_machine.hpp"

#include "system_state_map.hpp"

void SystemStateMachine::Init(StateMachine* state_machine, Context* context,
                              SystemStateMachine::Id init_state_id) {
  State* next_state;
  state_machine->context = context;
  next_state = SystemStateConfigMap_Get(init_state_id);
  if (next_state != NULL) {
    state_machine->current_state = next_state;
    SystemStateMachine::TriggerOnEnter(state_machine);
  }
}

void SystemStateMachine::TryTransition(StateMachine* state_machine,
                                       SystemStateMachine::ActionId action_id) {
  SystemStateMachine::Id next_state_id;
  State* next_state;
  next_state_id = GetNextSystemStateId(state_machine->current_state, action_id);
  if (next_state_id == SystemStateMachine::Id::NONE) {
    return;
  }
  next_state = SystemStateConfigMap_Get(next_state_id);
  if (next_state != NULL) {
    state_machine->current_state = next_state;
    SystemStateMachine::TriggerOnEnter(state_machine);
  }
}

void SystemStateMachine::TriggerOnEnter(StateMachine* state_machine) {
  SystemStateOnEnterResult result =
      state_machine->current_state->OnEnter(state_machine->context);
  if (result.is_transition_requested) {
    SystemStateMachine::TryTransition(state_machine, result.action_id);
  }
}
