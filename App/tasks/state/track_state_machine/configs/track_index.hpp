#ifndef TRACK_INDEX_HPP
#define TRACK_INDEX_HPP

#include <cstdint>

#include "track_config.h"

enum class TrackIndex : std::uint8_t {
  TRACK_1 = 0,
  TRACK_2 = 1,
  TRACK_3 = 2,
  TRACK_4 = 3,
  TRACK_5 = 4,
  COUNT,
};

std::size_t ToIndex(TrackIndex track_index);

#endif
