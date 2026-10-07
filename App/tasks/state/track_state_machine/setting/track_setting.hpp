#ifndef TRACK_SETTING_MAP_HPP
#define TRACK_SETTING_MAP_HPP

#include "track_measure.hpp"
#include "track_play_level.hpp"
#include "track_start_mode.hpp"
#include "track_stop_mode.hpp"

class TrackSetting {
 public:
  TrackSetting()
      : play_level(100),
        start_mode(TrackStartMode::IMMIDIATE),
        stop_mode(TrackStopMode::IMMIDIATE),
        fade_in_measure(TrackMeasure::DOUBLE_WHOLE_NOTE) {};

  TrackStartMode GetStartMode() const { return start_mode; }
  TrackStopMode GetStopMode() const { return stop_mode; }
  TrackPlayLevel& GetPlayLevel() { return play_level; }
  TrackMeasure GetTrackFadeInMeasure() { return fade_in_measure; }

 private:
  TrackPlayLevel play_level;
  TrackStartMode start_mode;
  TrackStopMode stop_mode;
  TrackMeasure fade_in_measure;
};

#endif
