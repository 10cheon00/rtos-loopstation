#ifndef TRACK_STATE_MACHINE_HPP
#define TRACK_STATE_MACHINE_HPP

#include "track_state.hpp"
#include "track_state_machine_context.hpp"

namespace TrackStateMachine {

struct StateMachine {
  State* current_state;
  Context* context;
};

void Init(StateMachine* state_machine,
                            Context* context,
                            Id init_state_id);
void TryTransition(StateMachine* state_machine,
                                     ActionId action_id);

}  // namespace TrackStateMachine

#endif
