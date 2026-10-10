#ifndef PARAMETER_VALUE_HPP
#define PARAMETER_VALUE_HPP

#include <cstdint>
#include <variant>

#include "parameter_toggle_value.hpp"
#include "parameter_unsigned_range_value.hpp"

namespace ParameterModel {

using ValueVariant = std::variant<std::monostate, UnsignedRangeValue, ToggleValue>;

}  // namespace ParameterModel

#endif
