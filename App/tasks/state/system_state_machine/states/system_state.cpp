#include "system_state.hpp"

namespace SystemStateMachine {

Id GetNextSystemStateId(State* state, ActionId action_id) {
  return (*state->transition_table)[action_id];
}

}  // namespace SystemStateMachine
