#ifndef TRACK_STATE_HPP
#define TRACK_STATE_HPP

#include <cstdint>

#include "enum_map.hpp"
#include "id.h"
#include "track_state_id.hpp"
#include "track_state_machine_context.hpp"

namespace TrackStateMachine {

enum class ActionId : std::uint8_t {
  NONE = ID_NONE,
  ENTER_RECORD_PLAY,
  ENTER_STOP,
  COUNT,
};

using TransitionEntry = EnumEntry<ActionId, Id>;
using TransitionEntryArray =
    std::array<TransitionEntry, static_cast<size_t>(ActionId::COUNT)>;

class State {
 protected:
  template <typename... TransitionEntries>
  explicit State(Id id, TransitionEntries... transition_entries)
      : id(id), transition_table(transition_entries...) {};

 public:
  const Id GetTrackStateId(ActionId action_id) const {
    return this->transition_table[action_id];
  }

  virtual void OnEnter(Context* contex) {}
  const Id GetId() { return this->id; }

 private:
  const Id id;
  const EnumMap<ActionId, Id> transition_table;
};

}  // namespace TrackStateMachine

#endif
