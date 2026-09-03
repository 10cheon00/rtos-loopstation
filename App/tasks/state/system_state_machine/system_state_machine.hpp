#ifndef SYSTEM_STATE_MACHINE_H
#define SYSTEM_STATE_MACHINE_H

#include "system_state.hpp"
#include "system_state_machine_context.hpp"

namespace SystemStateMachine {

struct StateMachine {
  State* current_state;
  Context* context;
};

void Init(StateMachine* state_machine, Context* context,
          Id init_state_id);
void TryTransition(StateMachine* state_machine, ActionId action_id);
void TriggerOnEnter(StateMachine* state_machine);

}  // namespace SystemStateMachine

#endif
