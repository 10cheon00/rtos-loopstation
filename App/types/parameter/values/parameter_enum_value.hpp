#ifndef PARAMETER_ENUM_VALUE_HPP
#define PARAMETER_ENUM_VALUE_HPP

#include <cstdint>
#include <type_traits>

#include "rtos_parameter_copy.h"

namespace ParameterModel {

template <typename Enum>
struct EnumValue {
  Enum min;
  Enum value;
  Enum max;
  const char** enum_name_table;

  constexpr EnumValue(Enum enum_value, const char** enum_name_table)
      : min(static_cast<Enum>(0)),
        value(enum_value),
        max(static_cast<Enum>(
            static_cast<std::underlying_type_t<Enum>>(Enum::COUNT) - 1)),
        enum_name_table(enum_name_table) {}
  constexpr EnumValue(
      RtosParameterValue_EnumValue rtos_parameter_value_enum_value)
      : min(static_cast<Enum>(rtos_parameter_value_enum_value.enum_min)),
        value(static_cast<Enum>(rtos_parameter_value_enum_value.enum_value)),
        max(static_cast<Enum>(rtos_parameter_value_enum_value.enum_max)),
        enum_name_table(rtos_parameter_value_enum_value.enum_name_table) {}
};

template <typename T>
struct IsEnumValue : std::false_type {};

template <typename E>
struct IsEnumValue<EnumValue<E>> : std::true_type {};

template <typename T>
inline constexpr bool IsEnumValueV = IsEnumValue<std::decay_t<T>>::value;

}  // namespace ParameterModel

#endif
