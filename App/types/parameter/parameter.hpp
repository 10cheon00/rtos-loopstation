#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <cstdbool>
#include <cstdint>
#include <type_traits>
#include <variant>

#include "enum_id.hpp"
#include "parameter_type.hpp"
#include "parameter_value.hpp"
#include "rtos_parameter_copy.h"
#include "utils.hpp"

namespace ParameterModel {

class Parameter {
 public:
  constexpr Parameter() : value_variant() {}

  constexpr Parameter(ValueVariant& value_variant)
      : value_variant(value_variant) {}

  Parameter(RtosParameterCopy& copy) {
    ParameterType type = Utils::FromRtosEnumValue<ParameterType>(
        copy.rtos_enum_value_parameter_type);
    switch (type) {
      case ParameterType::UNSIGNED_RANGE:
        value_variant = UnsignedRangeValue(
            copy.rtos_parameter_union.rtos_parameter_unsigned_range_value);
        break;
      case ParameterType::TOGGLE:
        value_variant =
            ToggleValue(copy.rtos_parameter_union.rtos_parameter_toggle_value);
        break;
      case ParameterType::SELECTED_FX:
        value_variant = FxSelectedFxEnumValue(
            copy.rtos_parameter_union.rtos_parameter_enum_value);
        break;
      case ParameterType::MEASURE_WITH_UNSIGNED_RANGE:
        value_variant = MeasureWithUnsignedRangeValue(
            copy.rtos_parameter_union
                .rtos_parameter_measure_with_unsigned_range_value);
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
          } else if constexpr (IsEnumValueV<T>) {
            using Enum = std::decay_t<decltype(value.value)>;
            const auto next = static_cast<std::int16_t>(value.value) + amount;
            const auto min = static_cast<std::int16_t>(value.min);
            const auto max = static_cast<std::int16_t>(value.max);
            value.value = static_cast<Enum>(next < min   ? min
                                            : next > max ? max
                                                         : next);
          } else if constexpr (std::is_same_v<T,
                                              MeasureWithUnsignedRangeValue>) {
            value.current = value.clamp(value.current + amount);
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
                Utils::ToRtosEnumValue(ParameterType::UNSIGNED_RANGE);
            copy.rtos_parameter_union.rtos_parameter_unsigned_range_value = {
                .min = value.min,
                .max = value.max,
                .current = value.current,
            };
          } else if constexpr (std::is_same_v<T, ToggleValue>) {
            copy.rtos_enum_value_parameter_type =
                Utils::ToRtosEnumValue(ParameterType::TOGGLE);
            copy.rtos_parameter_union.rtos_parameter_toggle_value = {
                .is_on = value.is_on,
            };
          } else if constexpr (std::is_same_v<T, FxSelectedFxEnumValue>) {
            copy.rtos_enum_value_parameter_type =
                Utils::ToRtosEnumValue(ParameterType::SELECTED_FX);
            copy.rtos_parameter_union.rtos_parameter_enum_value = {
                .enum_min = static_cast<RtosParameterValue>(value.min),
                .enum_value = static_cast<RtosParameterValue>(value.value),
                .enum_max = static_cast<RtosParameterValue>(value.max),
                .enum_name_table = value.enum_name_table,
            };
          } else if constexpr (std::is_same_v<T,
                                              MeasureWithUnsignedRangeValue>) {
            copy.rtos_enum_value_parameter_type = Utils::ToRtosEnumValue(
                ParameterType::MEASURE_WITH_UNSIGNED_RANGE);
            copy.rtos_parameter_union
                .rtos_parameter_measure_with_unsigned_range_value = {
                .min = value.min,
                .max = value.max,
                .current = value.current,
            };
          } else {
            copy.rtos_enum_value_parameter_type =
                Utils::ToRtosEnumValue(ParameterType::NONE);
          }
        },
        value_variant);
  }

  ValueVariant& GetValueVariant() { return value_variant; }

  template <typename ValueType>
  ValueType& GetValue() {
    return std::get<ValueType>(GetValueVariant());
  }

 private:
  ValueVariant value_variant;
};  // namespace ParameterModel

}  // namespace ParameterModel

#endif
