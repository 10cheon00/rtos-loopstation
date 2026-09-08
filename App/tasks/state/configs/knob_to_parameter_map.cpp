#include "knob_to_parameter_map.hpp"

#include "enum_map.hpp"

namespace KnobToParameterMap {

static constexpr EnumMap<KnobId, ParameterId> knob_parameter_map{
    EnumEntry{KnobId::NONE, ParameterId::NONE},
    EnumEntry{KnobId::IFX, ParameterId::IFX_KNOB},
    EnumEntry{KnobId::TFX, ParameterId::TFX_KNOB},
    EnumEntry{KnobId::TRACK_1_VOLUME_FADER, ParameterId::TRACK_1_VOLUME},
    EnumEntry{KnobId::TRACK_2_VOLUME_FADER, ParameterId::NONE},
    EnumEntry{KnobId::TRACK_3_VOLUME_FADER, ParameterId::NONE},
    EnumEntry{KnobId::TRACK_4_VOLUME_FADER, ParameterId::NONE},
    EnumEntry{KnobId::TRACK_5_VOLUME_FADER, ParameterId::NONE},
};

ParameterId Get(KnobId id) { return knob_parameter_map[id]; }

}  // namespace KnobToParameterMap
