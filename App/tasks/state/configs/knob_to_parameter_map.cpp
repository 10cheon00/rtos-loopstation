#include "knob_to_parameter_map.hpp"

#include "enum_map.hpp"

namespace KnobToParameterMap {

static constexpr EnumMap<KnobId, ParameterModel::Id> knob_parameter_map{
    EnumEntry{KnobId::NONE, ParameterModel::Id::NONE},
    EnumEntry{KnobId::IFX, ParameterModel::Id::IFX_KNOB},
    EnumEntry{KnobId::TFX, ParameterModel::Id::TFX_KNOB},
    EnumEntry{KnobId::TRACK_1_VOLUME_FADER, ParameterModel::Id::TRACK_1_VOLUME},
    EnumEntry{KnobId::TRACK_2_VOLUME_FADER, ParameterModel::Id::NONE},
    EnumEntry{KnobId::TRACK_3_VOLUME_FADER, ParameterModel::Id::NONE},
    EnumEntry{KnobId::TRACK_4_VOLUME_FADER, ParameterModel::Id::NONE},
    EnumEntry{KnobId::TRACK_5_VOLUME_FADER, ParameterModel::Id::NONE},
};

ParameterModel::Id Get(KnobId id) { return knob_parameter_map[id]; }

}  // namespace KnobToParameterMap
