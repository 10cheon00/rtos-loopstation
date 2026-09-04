#ifndef TRACK_STATE_MACHINE_HPP
#define TRACK_STATE_MACHINE_HPP

#include "track_state.hpp"
#include "track_state_machine_context.hpp"
#include "track_state_pointer_map.hpp"

namespace TrackStateMachine {

using namespace TrackStatePointerMap;

class StateMachine {
 public:
  StateMachine(Context& context, Id id) : context(context) {
    TrackStatePointer next_track_state = Get(id);
    if (next_track_state != nullptr) {
      this->current_state = next_track_state;
    }
  }

  TrackStatePointer GetCurrentState() const { return this->current_state; }
  Context& GetContext() const { return this->context; }

  void TryTransition(ActionId action_id) {
    Id next_state_id = this->current_state->GetTrackStateId(action_id);
    State* next_state = TrackStatePointerMap::Get(next_state_id);
    if (next_state == NULL) {
      // TODO:
      // 전이가 안된 경우에 대해 예외처리 하기
      return;
    }
    this->current_state = next_state;
    this->current_state->OnEnter(this->context);
  }

 private:
  TrackStatePointer current_state;
  Context& context;
};

}  // namespace TrackStateMachine

#endif
