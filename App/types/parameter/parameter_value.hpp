#ifndef PARAMETER_VALUE_HPP
#define PARAMETER_VALUE_HPP

#include <cstdint>
#include <variant>

#include "parameter_toggle_value.hpp"
#include "parameter_unsigned_range_value.hpp"
#include "parameter_enum_value.hpp"
#include "fx_section.hpp"
#include "fx_slot.hpp"

namespace ParameterModel {

using ValueVariant = std::variant<std::monostate, UnsignedRangeValue, ToggleValue, EnumValue<Fx::Section>, EnumValue<Fx::Slot>>;

}  // namespace ParameterModel

#endif
