#include "system_parameter_store.hpp"

#include <cstddef>

#include "parameter_value.hpp"

namespace Store {

using namespace ParameterModel;

void SystemParameterStore::Reset() {
  system_parameters = EnumMap<ParameterModel::Id, ParameterModel::Parameter>{
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
  };
}

ParameterModel::Parameter& SystemParameterStore::Get(ParameterModel::Id id) {
  return system_parameters.Get(id);
}

}  // namespace Store
