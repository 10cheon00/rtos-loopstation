#ifndef KNOB_TO_ID_MAP_HPP
#define KNOB_TO_ID_MAP_HPP

#include <cstddef>
#include <variant>

#include "enum_map.hpp"
#include "knob_id.hpp"
#include "parameter_id.hpp"
#include "track_index.hpp"
#include "track_setting_id.hpp"

namespace KnobToIdVariantMap {

struct TrackAdcControl {
  TrackSettingId id;
  TrackIndex index;
};

using IdVariant = std::variant<ParameterId, TrackAdcControl>;

IdVariant Get(KnobId id);

}  // namespace KnobToIdVariantMap

#endif
