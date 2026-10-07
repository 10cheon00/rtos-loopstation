#ifndef TRACK_SETTING_ID_HPP
#define TRACK_SETTING_ID_HPP

#include <cstdint>

enum class TrackSettingId : std::uint8_t {
  NONE = 0,
  REVERSE,
  PLAY_LEVEL,
  PAN,
  ONE_SHOT,
  APPLY_TFX,
  PLAY_MODE,
  START_MODE,
  STOP_MODE,
  MEASURE,
  LOOP_SYNC,
  TEMPO_SYNC,
  COUNT,
};

#endif
