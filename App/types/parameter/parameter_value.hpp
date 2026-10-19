#ifndef PARAMETER_VALUE_HPP
#define PARAMETER_VALUE_HPP

#include <cstdint>
#include <variant>

#include "fx_section.hpp"
#include "fx_slot.hpp"
#include "fx_type.hpp"
#include "parameter_enum_value.hpp"
#include "parameter_toggle_value.hpp"
#include "parameter_unsigned_range_value.hpp"

namespace ParameterModel {

// 파라미터에 쓰이는 모든 Enum들을 여기에 정의한다.
using FxSelectedSectionEnumValue = EnumValue<Fx::Section>;
using FxSelectedSlotEnumValue = EnumValue<Fx::Slot>;
using FxSelectedFxEnumValue = EnumValue<Fx::Type>;

using ValueVariant =
    std::variant<std::monostate, UnsignedRangeValue, ToggleValue,
                 FxSelectedSectionEnumValue, FxSelectedSlotEnumValue, FxSelectedFxEnumValue>;

}  // namespace ParameterModel

#endif
