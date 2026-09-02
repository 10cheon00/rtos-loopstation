#ifndef TRACK_STATE_ID_HPP
#define TRACK_STATE_ID_HPP

#include "id.h"

enum class TrackStateId {
  NONE = ID_NONE,
  IDLE,
  RECORDING,
  STOPPED,
  PLAYING,
  OVERDUBBING,
  COUNT,
};


#endif
