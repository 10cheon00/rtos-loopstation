#ifndef PARAMETER_TOGGLE_VALUE_HPP
#define PARAMETER_TOGGLE_VALUE_HPP

#include <cstdbool>

#include "rtos_parameter_copy.h"

namespace ParameterModel {

struct ToggleValue {
  bool is_on;
  constexpr ToggleValue(bool is_on) : is_on(is_on) {}
  constexpr ToggleValue(RtosParameterValue_ToggleValue rtos_parameter_value)
      : ToggleValue(rtos_parameter_value.is_on) {}
};

}  // namespace ParameterModel

#endif
