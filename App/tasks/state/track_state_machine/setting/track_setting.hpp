#ifndef TRACK_SETTING_MAP_HPP
#define TRACK_SETTING_MAP_HPP

#include "track_play_level.hpp"
#include "track_start_mode.hpp"
#include "track_stop_mode.hpp"

class TrackSetting {
 public:
  TrackSetting()
      : start_mode(TrackStartMode::IMMIDIATE),
        stop_mode(TrackStopMode::IMMIDIATE) {};

  TrackStartMode GetStartMode() const { return start_mode; }
  TrackStopMode GetStopMode() const { return stop_mode; }
  TrackPlayLevel& GetPlayLevel() { return play_level; }

 private:
  TrackPlayLevel play_level;
  TrackStartMode start_mode;
  TrackStopMode stop_mode;
};

#endif
