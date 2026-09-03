#include "system_state_not_inited.hpp"

#include "mcp23017.hpp"

static constexpr SystemStateMachine::TransitionTable
    system_state_not_init_transition_table{
        EnumEntry{SystemStateMachine::ActionId::INITED,
                  SystemStateMachine::Id::RUNNING},
        EnumEntry{SystemStateMachine::ActionId::ERROR,
                  SystemStateMachine::Id::ERROR},
    };

static SystemStateMachine::SystemStateOnEnterResult
SystemStateNotInited_OnEnter(SystemStateMachine::Context* context);

SystemStateMachine::State SYSTEM_STATE_NOT_INITED = {
    .id = SystemStateMachine::Id::NOT_INITED,
    .transition_table = &system_state_not_init_transition_table,
    .OnEnter = SystemStateNotInited_OnEnter,
};

static SystemStateMachine::SystemStateOnEnterResult
SystemStateNotInited_OnEnter(SystemStateMachine::Context* context) {
  // TODO:
  // Mcp23017Driver 초기화 수행
  return {.action_id = SystemStateMachine::ActionId::INITED,
          .is_transition_requested = true};
}
