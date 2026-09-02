#ifndef TRACK_STATE_H
#define TRACK_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "id.h"
#include "state_id.h"
#include "track_state_id.hpp"
#include "track_state_machine_context.h"

// typedef StateId TrackStateId;

typedef enum {
  TRACK_ACTION_ID_NONE = ID_NONE,
  TRACK_ACTION_ID_NULL = ID_NULL,
  TRACK_ACTION_ID_ENTER_RECORD_PLAY,
  TRACK_ACTION_ID_ENTER_STOP,
  TRACK_ACTION_ID_COUNT,
} TrackActionId;

typedef void (*TrackStateOnEnterFunction)(TrackStateMachineContext*);

struct TrackState {
  TrackStateId id;
  const TrackStateId* transition_table;
  TrackStateOnEnterFunction OnEnter;
};

TrackStateId TrackState_GetTrackStateId(TrackState* track_state,
                                        TrackActionId action_id);

#ifdef __cplusplus
}
#endif

#endif