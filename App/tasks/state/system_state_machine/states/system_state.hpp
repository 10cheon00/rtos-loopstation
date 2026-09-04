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

using TransitionTable = EnumMap<ActionId, Id>;

class State {
 protected:
  template <typename... TransitionEntries>
  explicit State(Id id, TransitionEntries... transition_entries)
      : id(id), transition_table(transition_entries...) {};

 public:
  const Id GetId() { return this->id; }
  const Id GetNextStateId(ActionId action_id) const {
    return this->transition_table[action_id];
  }

  virtual void OnEnter(Context& context) = 0;

 private:
  const Id id;
  const EnumMap<ActionId, Id> transition_table;
};

// Id GetNextSystemStateId(State* state, ActionId action_id);

}  // namespace SystemStateMachine

#endif
