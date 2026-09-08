#ifndef TRACK_STATE_ID_HPP
#define TRACK_STATE_ID_HPP

#include <cstdint>

#include "id.h"

namespace TrackStateMachine {

enum class Id : std::uint8_t {
  NONE = ID_NONE,
  IDLE,
  RECORDING,
  STOPPED,
  PLAYING,
  OVERDUBBING,
  COUNT,
};

}

#endif
