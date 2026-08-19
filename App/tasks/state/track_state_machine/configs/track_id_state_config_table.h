#ifndef TRACK_ID_STATE_CONFIG_TABLE_H
#define TRACK_ID_STATE_CONFIG_TABLE_H

#include "track_state.h"

typedef TrackState *TrackStatePointer;

TrackStatePointer TrackIdStateConfigMap_Get(TrackStateId id);

#endif
