#ifndef TRACK_STATE_MACHINE_H
#define TRACK_STATE_MACHINE_H

#include "track_state.h"
#include "track_state_machine_context.h"

typedef struct {
  TrackState* current_state;
  TrackStateMachineContext* context;
} TrackStateMachine;

void TrackStateMachine_Init(TrackStateMachine* state_machine,
                            TrackStateMachineContext* context,
                            TrackStateId init_state_id);
void TrackStateMachine_TryTransition(TrackStateMachine* state_machine,
                                     TrackActionId action_id);

#endif
