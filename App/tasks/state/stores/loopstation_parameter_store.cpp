#include "loopstation_parameter_store.h"

#include <stddef.h>

static constexpr auto initial_parameters = [] {
  std::array<Parameter, PARAMETER_ID_COUNT> values{};
  values[PARAMETER_ID_NONE] = Parameter{0, 0, 0, PARAMETER_TYPE_SLIDER};
  values[PARAMETER_ID_SETTING_PANEL_CURSOR] =
      Parameter{0, 100, 0, PARAMETER_TYPE_SLIDER};
  values[PARAMETER_ID_TFX_KNOB] = Parameter{0, 100, 0, PARAMETER_TYPE_SLIDER};
  values[PARAMETER_ID_IFX_KNOB] = Parameter{0, 100, 0, PARAMETER_TYPE_SLIDER};
  values[PARAMETER_ID_IFX_A_STATE] = Parameter{0, 1, 0, PARAMETER_TYPE_TOGGLE};
  values[PARAMETER_ID_TFX_A_STATE] = Parameter{0, 1, 0, PARAMETER_TYPE_TOGGLE};
  values[PARAMETER_ID_SYSTEM_SETTING_LCD_CONSTRAST] =
      Parameter{10, 80, 80, PARAMETER_TYPE_SLIDER};
  return values;
}();

static LoopStationParameterStore loopstation_parameter_store{
    initial_parameters};

Parameter* LoopStationParameterStore_Get(ParameterId parameter_id) {
  if (parameter_id <= PARAMETER_ID_NONE || parameter_id >= PARAMETER_ID_COUNT) {
    return NULL;
  }
  return &loopstation_parameter_store.parameters[parameter_id];
}
