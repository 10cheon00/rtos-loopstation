#include "track_state_idle.h"

#include "utils.h"

static TrackStateId track_state_idle_transition_table[TRACK_ACTION_ID_COUNT] = {
    [TRACK_ACTION_ID_NONE] = TRACK_STATE_ID_IDLE,
    [TRACK_ACTION_ID_ENTER_RECORD_PLAY] = TRACK_STATE_ID_RECORDING,
    [TRACK_ACTION_ID_ENTER_STOP] = TRACK_STATE_ID_IDLE,
};

static void TrackStateIdle_OnEnter(TrackStateMachineContext *context);

TrackState TRACK_STATE_IDLE = {.id = TRACK_STATE_ID_IDLE,
                               .transition_table = track_state_idle_transition_table,
                               .OnEnter = TrackStateIdle_OnEnter};

static void TrackStateIdle_OnEnter(TrackStateMachineContext *context) {}
