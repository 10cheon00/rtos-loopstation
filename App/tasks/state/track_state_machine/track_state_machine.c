#include "track_state_machine.h"

#include "track_id_state_table.h"

void TrackStateMachine_Init(TrackStateMachine *state_machine, TrackStateMachineContext *context,
                            TrackStateId init_state_id)
{
    TrackState *next_state;

    state_machine->context = context;
    next_state = TrackIdStateTable_GetTrackStateFromId(init_state_id);
    state_machine->current_state = next_state;
    state_machine->current_state->OnEnter(state_machine->context);
}

void TrackStateMachine_TryTransition(TrackStateMachine *state_machine, TrackActionId action_id)
{
    TrackStateId next_state_id = TrackState_GetTrackStateId(state_machine->current_state, action_id);
    TrackState *next_state = TrackIdStateTable_GetTrackStateFromId(next_state_id);
    if (next_state == NULL) {
        // TODO:
        // 전이가 안된 경우에 대해 예외처리 하기
        return;
    }
    state_machine->current_state = next_state;
    state_machine->current_state->OnEnter(state_machine->context);
}
