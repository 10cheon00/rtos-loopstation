#ifndef TRACK_SETTING_MAP_HPP
#define TRACK_SETTING_MAP_HPP

#include "mutable_enum_map.hpp"
#include "track_setting.hpp"
#include "track_setting_id.hpp"

class TrackSettingMap : public MutableEnumMap<TrackSettingId, TrackSetting> {
 public:
  TrackSettingMap();
};

#endif
