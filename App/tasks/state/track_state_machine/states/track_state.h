#ifndef TRACK_STATE_H
#define TRACK_STATE_H

#include <stddef.h>

#include "id.h"
#include "state_id.h"
#include "track_state_machine_context.h"

#define TRACK_COUNT 1

// typedef StateId TrackStateId;

typedef enum {
    TRACK_STATE_ID_NONE = ID_NONE,
    TRACK_STATE_ID_IDLE,
    TRACK_STATE_ID_RECORDING,
    TRACK_STATE_ID_STOPPED,
    TRACK_STATE_ID_PLAYING,
    TRACK_STATE_ID_OVERDUBBING,
    TRACK_STATE_ID_COUNT,
} TrackStateId;

typedef enum {
    TRACK_ACTION_ID_NONE = ID_NONE,
    TRACK_ACTION_ID_NULL = ID_NULL,
    TRACK_ACTION_ID_ENTER_RECORD_PLAY,
    TRACK_ACTION_ID_ENTER_STOP,
    TRACK_ACTION_ID_COUNT,
} TrackActionId;

typedef void (*TrackStateOnEnterFunction)(TrackStateMachineContext *);

typedef struct {
    TrackStateId id;
    const TrackStateId *transition_table;
    TrackStateOnEnterFunction OnEnter;
} TrackState;

TrackStateId TrackState_GetTrackStateId(TrackState *track_state, TrackActionId action_id);

#endif
