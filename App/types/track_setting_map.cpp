#include "track_setting_map.hpp"

#include "enum_map.hpp"

static constexpr MutableEnumMap<TrackSettingId, TrackSetting>
    initial_track_settings{
        EnumEntry{TrackSettingId::APPLY_TFX,
                  TrackSetting{0, 1, 1, TrackSettingType::TOGGLE}},
        EnumEntry{TrackSettingId::PLAY_LEVEL,
                  TrackSetting{0, 100, 100, TrackSettingType::SLIDER}},
};

TrackSettingMap::TrackSettingMap()
    : MutableEnumMap<TrackSettingId, TrackSetting>(initial_track_settings) {}