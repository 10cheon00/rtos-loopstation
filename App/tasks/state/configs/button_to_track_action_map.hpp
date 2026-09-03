#ifndef BUTTON_TO_TRACK_ACTION_MAP_HPP
#define BUTTON_TO_TRACK_ACTION_MAP_HPP

#include <stddef.h>

#include "button_id.hpp"
#include "track_state.h"

namespace ButtonToTrackActionMap {

TrackActionId Get(ButtonId id);

}

#endif
