#ifndef KNOB_ID_H
#define KNOB_ID_H

#include <cstdint>

#include "id.h"

enum class KnobId : std::uint8_t {
  NONE = ID_NONE,
  IFX,
  TFX,
  TRACK_1_VOLUME_FADER,
  TRACK_2_VOLUME_FADER,
  TRACK_3_VOLUME_FADER,
  TRACK_4_VOLUME_FADER,
  TRACK_5_VOLUME_FADER,
  COUNT,
};

#endif
