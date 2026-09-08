#ifndef BUTTON_TO_TRACK_ACTION_MAP_HPP
#define BUTTON_TO_TRACK_ACTION_MAP_HPP

#include <stddef.h>

#include "button_id.hpp"
#include "track_state.hpp"

namespace ButtonToTrackActionMap {

TrackStateMachine::ActionId Get(ButtonId id);

}

#endif
