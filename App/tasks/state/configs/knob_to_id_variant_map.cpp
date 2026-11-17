#include "knob_to_id_variant_map.hpp"

#include "enum_map.hpp"

namespace KnobToIdVariantMap {

static constexpr EnumMap<KnobId, IdVariant> knob_parameter_map{
    {KnobId::NONE, IdVariant{ParameterId::NONE}},
    {KnobId::IFX, IdVariant{ParameterId::IFX_KNOB}},
    {KnobId::TFX, IdVariant{ParameterId::TFX_KNOB}},
    {KnobId::TRACK_1_VOLUME_FADER,
     IdVariant{TrackAdcControl{TrackSettingId::PLAY_LEVEL,
                               TrackIndex::TRACK_1}}},
    {KnobId::TRACK_2_VOLUME_FADER,
     IdVariant{TrackAdcControl{TrackSettingId::PLAY_LEVEL,
                               TrackIndex::TRACK_2}}},
    {KnobId::TRACK_3_VOLUME_FADER,
     IdVariant{TrackAdcControl{TrackSettingId::PLAY_LEVEL,
                               TrackIndex::TRACK_3}}},
    {KnobId::TRACK_4_VOLUME_FADER,
     IdVariant{TrackAdcControl{TrackSettingId::PLAY_LEVEL,
                               TrackIndex::TRACK_4}}},
    {KnobId::TRACK_5_VOLUME_FADER,
     IdVariant{TrackAdcControl{TrackSettingId::PLAY_LEVEL,
                               TrackIndex::TRACK_5}}},
};

IdVariant Get(KnobId id) { return knob_parameter_map[id]; }

}  // namespace KnobToIdVariantMap
