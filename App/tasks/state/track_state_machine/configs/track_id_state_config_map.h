#ifndef TRACK_ID_STATE_CONFIG_MAP_H
#define TRACK_ID_STATE_CONFIG_MAP_H

#include "track_state.h"

typedef TrackState *TrackStatePointer;

TrackStatePointer TrackIdStateConfigMap_Get(TrackStateId id);

#endif
