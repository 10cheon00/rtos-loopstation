#ifndef TRACK_SETTING_HPP
#define TRACK_SETTING_HPP

#include <cstdbool>
#include <cstdint>

#include "enum_id.hpp"
#include "mutable_enum_map.hpp"
#include "rtos_track_setting_copy.h"
#include "utils.h"

using TrackSettingValue = std::int16_t;

enum class TrackSettingType : EnumId {
  NONE = 0,
  SLIDER,
  TOGGLE,
};

class TrackSetting {
 public:
  constexpr TrackSetting()
      : min(0), max(0), current(0), type(TrackSettingType::NONE) {}

  constexpr TrackSetting(TrackSettingValue min, TrackSettingValue max,
                         TrackSettingValue initial_value, TrackSettingType type)
      : min(min), max(max), current(initial_value), type(type) {}

  constexpr TrackSetting(RtosTrackSettingCopy& raw)
      : TrackSetting(
            raw.min, raw.max, raw.current,
            static_cast<TrackSettingType>(raw.track_setting_type_raw)) {}

  constexpr TrackSetting& operator=(const TrackSetting& track_setting) {
    this->min = track_setting.min;
    this->max = track_setting.max;
    this->current = track_setting.current;
    this->type = track_setting.type;
    return *this;
  }

  constexpr void Add(const TrackSettingValue value) {
    this->current = this->clamp(static_cast<std::int32_t>(this->current) +
                                static_cast<std::int32_t>(value));
  }

  constexpr void Set(const TrackSettingValue value) { this->current = value; }

  constexpr void Toggle() {
    this->current = IsCurrentMinimum() ? this->max : this->min;
  }

  constexpr const bool IsCurrentMinimum() const {
    return this->current == this->min;
  }

  constexpr const bool IsCurrentMaximum() const {
    return this->current == this->max;
  }

  const void ToRtosTrackSettingCopy(RtosTrackSettingCopy& raw) {
    raw.min = this->min;
    raw.max = this->max;
    raw.current = this->current;
    raw.track_setting_type_raw = ToRtosEnumValue(this->type);
  }

  constexpr const TrackSettingValue GetCurrent() const { return this->current; }
  const RtosTrackSettingValue GetCurrentForRtos() const {
    return static_cast<RtosTrackSettingValue>(this->current);
  }
  const TrackSettingValue GetMin() const { return this->min; }
  const TrackSettingValue GetMax() const { return this->max; }
  const TrackSettingType GetType() const { return this->type; }

 private:
  TrackSettingValue min;
  TrackSettingValue max;
  TrackSettingValue current;
  TrackSettingType type;

  constexpr TrackSettingValue clamp(std::int32_t value) {
    std::int32_t min_int32 = static_cast<std::int32_t>(this->min),
                 max_int32 = static_cast<std::int32_t>(this->max);

    if (value < min_int32) {
      return this->min;
    }
    if (value > max_int32) {
      return this->max;
    }
    return value;
  }
};

#endif
