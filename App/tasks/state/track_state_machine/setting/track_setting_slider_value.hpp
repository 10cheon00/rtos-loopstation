#ifndef TRACK_SETTING_SLIDER_VALUE_HPP
#define TRACK_SETTING_SLIDER_VALUE_HPP

#include "rtos_track_setting_copy.h"
#include "track_setting.hpp"
#include "track_setting_value.hpp"

class TrackSettingSliderValue {
 public:
  TrackSettingSliderValue(TrackSettingValue min, TrackSettingValue max,
                          TrackSettingValue current)
      : min(min), max(max), current(current) {}

  const TrackSettingValue GetMin() { return min; }
  const TrackSettingValue GetMax() { return max; }
  TrackSettingValue GetCurrent() const { return current; }
  void SetCurrent(TrackSettingValue value) { this->current = value; }
  RtosTrackSettingValue GetCurrentForRtos() const {
    return static_cast<RtosTrackSettingValue>(current);
  }

 private:
  TrackSettingValue min;
  TrackSettingValue max;
  TrackSettingValue current;
};

#endif
