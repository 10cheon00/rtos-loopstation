#include "track_state_stopped.h"

#include "utils.h"

static TrackStateId track_state_stopped_transition_table[TRACK_ACTION_ID_COUNT] = {
    [TRACK_ACTION_ID_NONE] = TRACK_STATE_ID_STOPPED,
    [TRACK_ACTION_ID_ENTER_RECORD_PLAY] = TRACK_STATE_ID_PLAYING,
    [TRACK_ACTION_ID_ENTER_STOP] = TRACK_STATE_ID_STOPPED,
};

static void TrackStateStopped_OnEnter(TrackStateMachineContext *context);

TrackState TRACK_STATE_STOPPED = {.id = TRACK_STATE_ID_STOPPED,
                                  .transition_table = track_state_stopped_transition_table,
                                  .OnEnter = TrackStateStopped_OnEnter};

static void TrackStateStopped_OnEnter(TrackStateMachineContext *context) {}
