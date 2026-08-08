#include "track_state.h"

TrackStateId TrackState_GetTrackStateId(TrackState *track_state, TrackActionId action_id)
{
    if (action_id <= TRACK_ACTION_ID_NONE || action_id >= TRACK_ACTION_ID_COUNT) {
        action_id = TRACK_ACTION_ID_NONE;
    }
    return track_state->transition_table[action_id];
}
