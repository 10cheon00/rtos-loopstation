#ifndef PARAMETER_TOGGLE_VALUE_HPP
#define PARAMETER_TOGGLE_VALUE_HPP

#include <cstdbool>

namespace ParameterModel {

struct ToggleValue {
  bool is_on;
  constexpr ToggleValue(bool is_on) : is_on(is_on) {}
};

}  // namespace ParameterModel

#endif
