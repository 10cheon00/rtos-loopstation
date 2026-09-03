#include "system_state_error.hpp"

static SystemStateMachine::SystemStateOnEnterResult SystemStateError_OnEnter(
    SystemStateMachine::Context* context);

static constexpr SystemStateMachine::TransitionTable
    system_state_error_transition_table{};

SystemStateMachine::State SYSTEM_STATE_ERROR = {
    .id = SystemStateMachine::Id::ERROR,
    .transition_table = &system_state_error_transition_table,
    .OnEnter = SystemStateError_OnEnter,
};

static SystemStateMachine::SystemStateOnEnterResult SystemStateError_OnEnter(
    SystemStateMachine::Context* context) {
  return {.action_id = SystemStateMachine::ActionId::NONE,
          .is_transition_requested = false};
}
