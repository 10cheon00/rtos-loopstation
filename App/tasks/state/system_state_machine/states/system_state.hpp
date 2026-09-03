#ifndef SYSTEM_STATE_HPP
#define SYSTEM_STATE_HPP

#include <cstdbool>
#include <cstdint>

#include "enum_map.hpp"
#include "system_state_machine_context.hpp"

namespace SystemStateMachine {

enum class Id : std::uint8_t {
  NONE = 0,
  NOT_INITED,
  RUNNING,
  ERROR,
  COUNT,
};

enum class ActionId : std::uint8_t {
  NONE = 0,
  INITED,
  ERROR,
  COUNT,
};

struct SystemStateOnEnterResult {
  ActionId action_id;
  bool is_transition_requested;
};

using SystemStateOnEnterFunction = SystemStateOnEnterResult (*)(Context*);
using TransitionTable = EnumMap<ActionId, Id>;

struct State {
  Id id;
  const TransitionTable* transition_table;
  SystemStateOnEnterFunction OnEnter;
};

Id GetNextSystemStateId(State* state, ActionId action_id);

}  // namespace SystemStateMachine

#endif
