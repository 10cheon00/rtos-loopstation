#include "track_id_state_table.h"

#include "track_state_idle.h"
#include "track_state_recording.h"
#include "track_state_stopped.h"
#include "track_state_playing.h"
#include "track_state_overdubbing.h"

static TrackState *track_id_state_table[TRACK_STATE_ID_COUNT] = {
    [TRACK_STATE_ID_NONE] = NULL,
    [TRACK_STATE_ID_IDLE] = &TRACK_STATE_IDLE,
    [TRACK_STATE_ID_RECORDING] = &TRACK_STATE_RECORDING,
    [TRACK_STATE_ID_STOPPED] = &TRACK_STATE_STOPPED,
    [TRACK_STATE_ID_PLAYING] = &TRACK_STATE_PLAYING,
    [TRACK_STATE_ID_OVERDUBBING] = &TRACK_STATE_OVERDUBBING
};

TrackState *TrackIdStateTable_GetTrackStateFromId(TrackStateId id)
{
    if (id <= TRACK_STATE_ID_NONE || id >=TRACK_STATE_ID_COUNT) {
        id = TRACK_ACTION_ID_NONE;
    }
    return track_id_state_table[id];
}
