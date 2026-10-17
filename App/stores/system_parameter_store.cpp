#include "system_parameter_store.hpp"

#include <cstddef>

#include "mutex_guard.hpp"
#include "parameter_value.hpp"
#include "fx_section.hpp"
#include "fx_slot.hpp"

namespace Store {

using namespace ParameterModel;

static constexpr EnumMap<ParameterModel::Id, ParameterModel::Parameter>
    initial_parameters{
        EnumEntry{
            Id::NONE,
            ValueVariant(),
        },
        EnumEntry{
            Id::IFX_KNOB,
            ValueVariant(UnsignedRangeValue(0, 100, 100)),
        },
        EnumEntry{
            Id::TFX_KNOB,
            ValueVariant(UnsignedRangeValue(0, 100, 100)),
        },
        EnumEntry{
            Id::IFX_A_STATE,
            ValueVariant(ToggleValue(false)),
        },
        EnumEntry{
            Id::TFX_A_STATE,
            ValueVariant(ToggleValue(false)),
        },
        EnumEntry{
            Id::SYSTEM_SETTING_LCD_CONSTRAST,
            ValueVariant(UnsignedRangeValue(10, 80, 80)),
        },
        EnumEntry{
            Id::TRACK_1_VOLUME,
            ValueVariant(UnsignedRangeValue(0, 100, 100)),
        },
        EnumEntry{
            Id::SELECTED_FX_SECTION,
            ValueVariant(EnumValue(Fx::Section::IFX)),
        },
        EnumEntry{
            Id::SELECTED_FX_SLOT,
            ValueVariant(EnumValue(Fx::Slot::A)),
        },
};

void SystemParameterStore::Reset(osMutexId_t mutex) {
  system_parameters = initial_parameters;
  this->mutex = mutex;
}

ParameterModel::Parameter& SystemParameterStore::Get(ParameterModel::Id id) {
  MutexGuard lock{mutex, 500UL};
  return system_parameters.Get(id);
}

}  // namespace Store
