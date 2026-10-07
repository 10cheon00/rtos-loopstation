#ifndef TRACK_PLAY_LEVEL_HPP
#define TRACK_PLAY_LEVEL_HPP

#include "track_setting_slider_value.hpp"

class TrackPlayLevel : public TrackSettingSliderValue {
 public:
  TrackPlayLevel(TrackSettingValue init_current)
      : TrackSettingSliderValue(0, 100, init_current) {}
};

#endif
