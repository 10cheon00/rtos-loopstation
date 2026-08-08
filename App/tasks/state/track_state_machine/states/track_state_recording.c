#include "track_state_recording.h"

#include "utils.h"

static TrackStateId track_state_recording_transition_table[TRACK_ACTION_ID_COUNT] = {
    [TRACK_ACTION_ID_NONE] = TRACK_STATE_ID_RECORDING,
    [TRACK_ACTION_ID_ENTER_RECORD_PLAY] = TRACK_STATE_ID_PLAYING,
    [TRACK_ACTION_ID_ENTER_STOP] = TRACK_STATE_ID_STOPPED,
};

static void TrackStateRecording_OnEnter(TrackStateMachineContext *context);

TrackState TRACK_STATE_RECORDING = {.id = TRACK_STATE_ID_RECORDING,
                                    .transition_table = track_state_recording_transition_table,
                                    .OnEnter = TrackStateRecording_OnEnter};

static void TrackStateRecording_OnEnter(TrackStateMachineContext *context) {}
