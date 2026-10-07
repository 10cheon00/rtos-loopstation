#ifndef TRACK_PLAY_LEVEL_HPP
#define TRACK_PLAY_LEVEL_HPP

#include "track_setting_slider_value.hpp"

class TrackPlayLevel : public TrackSettingSliderValue {
 public:
  TrackPlayLevel() : TrackSettingSliderValue(0, 100, 100) {}
};

#endif
