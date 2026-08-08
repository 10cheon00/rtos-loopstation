#ifndef TRACK_STATE_H
#define TRACK_STATE_H

#include <stddef.h>

#include "state_id.h"
#include "track_state_machine_context.h"

#define TRACK_COUNT 1

typedef StateId TrackStateId;

enum TrackStateId {
    TRACK_STATE_ID_NONE = 0,
    TRACK_STATE_ID_IDLE,
    TRACK_STATE_ID_RECORDING,
    TRACK_STATE_ID_STOPPED,
    TRACK_STATE_ID_PLAYING,
    TRACK_STATE_ID_OVERDUBBING,
    TRACK_STATE_ID_COUNT,
};

typedef enum {
    TRACK_ACTION_ID_NONE = 0,
    TRACK_ACTION_ID_ENTER_RECORD_PLAY,
    TRACK_ACTION_ID_ENTER_STOP,
    TRACK_ACTION_ID_COUNT,
} TrackActionId;

typedef void (*OnEnterFunction)(TrackStateMachineContext *);

typedef struct {
    TrackStateId id;
    TrackStateId *transition_table;
    OnEnterFunction OnEnter;
} TrackState;

TrackStateId TrackState_GetTrackStateId(TrackState *track_state, TrackActionId action_id);

#endif
