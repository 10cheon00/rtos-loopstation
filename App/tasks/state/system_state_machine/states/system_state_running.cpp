#include "system_state_running.hpp"

static constexpr SystemStateMachine::TransitionTable
    system_state_running_transition_table{
        EnumEntry{SystemStateMachine::ActionId::ERROR,
                  SystemStateMachine::Id::ERROR},
    };

static SystemStateMachine::SystemStateOnEnterResult SystemStateRunning_OnEnter(
    SystemStateMachine::Context* context);

SystemStateMachine::State SYSTEM_STATE_RUNNING = {
    .id = SystemStateMachine::Id::RUNNING,
    .transition_table = &system_state_running_transition_table,
    .OnEnter = SystemStateRunning_OnEnter,
};

static SystemStateMachine::SystemStateOnEnterResult SystemStateRunning_OnEnter(
    SystemStateMachine::Context* context) {
  return {.action_id = SystemStateMachine::ActionId::NONE,
          .is_transition_requested = false};
}
