#ifndef PARAMETER_ENUM_VALUE_HPP
#define PARAMETER_ENUM_VALUE_HPP

#include <cstdint>
#include <type_traits>

#include "rtos_parameter_copy.h"

namespace ParameterModel {

template <typename Enum>
struct EnumValue {
  Enum value;

  constexpr EnumValue(Enum enum_value) : value(enum_value) {}
  constexpr EnumValue(
      RtosParameterValue_EnumValue rtos_parameter_value_enum_value)
      : value(static_cast<Enum>(rtos_parameter_value_enum_value.enum_value)) {}
};

template <typename T>
struct IsEnumValue : std::false_type {};

template <typename E>
struct IsEnumValue<EnumValue<E>> : std::true_type {};

template <typename T>
inline constexpr bool IsEnumValueV = IsEnumValue<std::decay_t<T>>::value;

}  // namespace ParameterModel

#endif
