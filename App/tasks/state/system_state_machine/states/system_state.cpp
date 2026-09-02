#include "system_state.h"

SystemStateId SystemState_GetNextSystemStateId(SystemState* state,
                                               SystemActionId action_id) {
  if (action_id <= SYSTEM_ACTION_ID_NONE ||
      action_id >= SYSTEM_ACTION_ID_COUNT) {
    action_id = SYSTEM_ACTION_ID_NONE;
  }
  return state->transition_table[action_id];
}
