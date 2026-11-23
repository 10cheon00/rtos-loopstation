#include "knob_to_parameter_map.hpp"

#include "enum_map.hpp"

namespace KnobToParameterMap {

static constexpr EnumMap<KnobId, ParameterModel::Id> knob_parameter_map{
    {KnobId::NONE, ParameterModel::Id::NONE},
    {KnobId::IFX, ParameterModel::Id::IFX_KNOB},
    {KnobId::TFX, ParameterModel::Id::TFX_KNOB},
    {KnobId::TRACK_1_VOLUME_FADER, ParameterModel::Id::TRACK_1_VOLUME},
    {KnobId::TRACK_2_VOLUME_FADER, ParameterModel::Id::TRACK_2_VOLUME},
    {KnobId::TRACK_3_VOLUME_FADER, ParameterModel::Id::TRACK_3_VOLUME},
    {KnobId::TRACK_4_VOLUME_FADER, ParameterModel::Id::TRACK_4_VOLUME},
    {KnobId::TRACK_5_VOLUME_FADER, ParameterModel::Id::TRACK_5_VOLUME},
};

ParameterModel::Id Get(KnobId id) { return knob_parameter_map[id]; }

}  // namespace KnobToParameterMap
