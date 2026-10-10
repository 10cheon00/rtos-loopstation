#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <cstdbool>
#include <cstdint>
#include <type_traits>
#include <variant>

#include "enum_id.hpp"
#include "parameter_type.h"
#include "parameter_value.hpp"
#include "rtos_parameter_copy.h"
#include "utils.h"

namespace ParameterModel {

class Parameter {
 public:
  constexpr Parameter() : value_variant() {}

  constexpr Parameter(UnsignedRangeValue value) : value_variant(value) {}

  constexpr Parameter(ToggleValue value) : value_variant(value) {}

  constexpr Parameter(ValueVariant& value_variant)
      : value_variant(value_variant) {}

  constexpr Parameter(RtosParameterCopy& copy) {
    switch (copy.rtos_enum_value_parameter_type) {
      case ParameterType::UNSIGNED_RANGE:
        value_variant = UnsignedRangeValue(
            copy.rtos_parameter_union.rtos_parameter_unsigned_range_value);
        break;
      case ParameterType::TOGGLE:
        value_variant =
            ToggleValue(copy.rtos_parameter_union.rtos_parameter_toggle_value);
        break;
      default:
        break;
    }
  }

  void Toggle() {
    std::visit(
        [](auto& value) {
          using T = std::decay_t<decltype(value)>;

          if constexpr (std::is_same_v<T, ToggleValue>) {
            value.is_on = !value.is_on;
          }
        },
        value_variant);
  }
  void Add(const std::int16_t amount) {
    std::visit(
        [amount](auto& value) {
          using T = std::decay_t<decltype(value)>;

          if constexpr (std::is_same_v<T, UnsignedRangeValue>) {
            value.current = value.clamp(value.current + amount);
          } else if constexpr (std::is_same_v<T, ToggleValue>) {
            value.is_on = amount > 0 ? true : false;
          }
        },
        value_variant);
  }
  void Set(const std::int16_t amount) {
    std::visit(
        [amount](auto& value) {
          using T = std::decay_t<decltype(value)>;
          if constexpr (std::is_same_v<T, UnsignedRangeValue>) {
            value.current = value.clamp(amount);
          } else if constexpr (std::is_same_v<T, ToggleValue>) {
            value.is_on = amount > 0 ? true : false;
          }
        },
        value_variant);
  }

  void ToRtosParameterCopy(RtosParameterCopy& copy) const {
    std::visit(
        [&copy](const auto& value) {
          using T = std::decay_t<decltype(value)>;

          if constexpr (std::is_same_v<T, UnsignedRangeValue>) {
            copy.rtos_enum_value_parameter_type =
                ToRtosEnumValue(ParameterType::UNSIGNED_RANGE);
            copy.rtos_parameter_union.rtos_parameter_unsigned_range_value = {
                .min = value.min,
                .max = value.max,
                .current = value.current,
            };
          } else if constexpr (std::is_same_v<T, ToggleValue>) {
            copy.rtos_enum_value_parameter_type =
                ToRtosEnumValue(ParameterType::TOGGLE);
            copy.rtos_parameter_union.rtos_parameter_toggle_value = {
                .is_on = value.is_on,
            };
          } else {
            copy.rtos_enum_value_parameter_type =
                ToRtosEnumValue(ParameterType::NONE);
          }
        },
        value_variant);
  }

  ValueVariant& GetValueVariant() { return value_variant; }

 private:
  ValueVariant value_variant;
};  // namespace ParameterModel

}  // namespace ParameterModel

#endif
