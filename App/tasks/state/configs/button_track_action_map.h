#ifndef BUTTON_TRACK_ACTION_MAP_H
#define BUTTON_TRACK_ACTION_MAP_H

#include <stddef.h>

#include "button_id.h"
#include "track_state.h"

typedef struct {
    ButtonId button_id;
    TrackActionId track_action_id;
} ButtonTrackActionMapEntry;

TrackActionId ButtonTrackActionMap_GetTrackActionId(ButtonId id);

#endif
