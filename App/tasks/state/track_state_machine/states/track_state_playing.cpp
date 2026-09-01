#include "track_state_playing.h"

#include "utils.h"

static TrackStateId track_state_playing_transition_table[TRACK_ACTION_ID_COUNT] = {
    [TRACK_ACTION_ID_NONE] = TRACK_STATE_ID_PLAYING,
    [TRACK_ACTION_ID_ENTER_RECORD_PLAY] = TRACK_STATE_ID_OVERDUBBING,
    [TRACK_ACTION_ID_ENTER_STOP] = TRACK_STATE_ID_STOPPED,
};

static void TrackStatePlaying_OnEnter(TrackStateMachineContext *context);

TrackState TRACK_STATE_PLAYING = {.id = TRACK_STATE_ID_PLAYING,
                                  .transition_table = track_state_playing_transition_table,
                                  .OnEnter = TrackStatePlaying_OnEnter};

static void TrackStatePlaying_OnEnter(TrackStateMachineContext *context) {}
