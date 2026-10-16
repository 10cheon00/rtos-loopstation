#include "system_parameter_store.hpp"

#include <cstddef>

#include "mutex_guard.hpp"
#include "parameter_value.hpp"

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
            UnsignedRangeValue(0, 100, 100),
        },
        EnumEntry{
            Id::TFX_KNOB,
            UnsignedRangeValue(0, 100, 100),
        },
        EnumEntry{
            Id::IFX_A_STATE,
            ToggleValue(false),
        },
        EnumEntry{
            Id::TFX_A_STATE,
            ToggleValue(false),
        },
        EnumEntry{
            Id::SYSTEM_SETTING_LCD_CONSTRAST,
            UnsignedRangeValue(10, 80, 80),
        },
        EnumEntry{
            Id::TRACK_1_VOLUME,
            UnsignedRangeValue(0, 100, 100),
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
