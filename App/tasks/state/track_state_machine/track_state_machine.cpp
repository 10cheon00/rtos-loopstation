#include "track_state_machine.hpp"

#include "track_state_pointer_map.hpp"

namespace TrackStateMachine {

void Init(StateMachine* state_machine, Context* context, Id init_state_id) {
  state_machine->context = context;

  TrackStatePointerMap::TrackStatePointer next_state =
      TrackStatePointerMap::Get(init_state_id);
  state_machine->current_state = next_state;
  state_machine->current_state->OnEnter(state_machine->context);
}

void TryTransition(StateMachine* state_machine, ActionId action_id) {
  Id next_state_id = state_machine->current_state->GetTrackStateId(action_id);
  State* next_state = TrackStatePointerMap::Get(next_state_id);
  if (next_state == NULL) {
    // TODO:
    // 전이가 안된 경우에 대해 예외처리 하기
    return;
  }
  state_machine->current_state = next_state;
  state_machine->current_state->OnEnter(state_machine->context);
}

}  // namespace TrackStateMachine
