#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <cstdbool>
#include <cstdint>

#include "enum_id.hpp"
#include "parameter_raw.h"
#include "utils.h"

using ParameterValue = std::int8_t;

enum class ParameterType : EnumId {
  NONE = 0,
  SLIDER,
  TOGGLE,
};

class Parameter {
 public:
  constexpr Parameter()
      : min(0), max(0), current(0), type(ParameterType::NONE) {}

  constexpr Parameter(ParameterValue min, ParameterValue max,
                      ParameterValue initial_value, ParameterType type)
      : min(min), max(max), current(initial_value), type(type) {}

  constexpr Parameter(ParameterRaw& raw)
      : Parameter(raw.min, raw.max, raw.current,
                  static_cast<ParameterType>(raw.parameter_type_raw)) {}

  constexpr Parameter& operator=(const Parameter& parameter) {
    this->min = parameter.min;
    this->max = parameter.max;
    this->current = parameter.current;
    this->type = parameter.type;
    return *this;
  }

  constexpr void Add(const ParameterValue value) {
    this->current = this->clamp(static_cast<std::int32_t>(this->current) +
                                static_cast<std::int32_t>(value));
  }

  constexpr void Toggle() {
    this->current = IsCurrentMinimum() ? this->max : this->min;
  }

  constexpr const ParameterValue GetCurrent() const { return this->current; }

  constexpr const bool IsCurrentMinimum() const {
    return this->current == this->min;
  }

  constexpr const bool IsCurrentMaximum() const {
    return this->current == this->max;
  }

  const void ToRaw(ParameterRaw& raw) {
    raw.min = this->min;
    raw.max = this->max;
    raw.current = this->current;
    raw.parameter_type_raw = ToRtosEnumValue(this->type);
  }

  const ParameterValue GetMin() const { return this->min; }
  const ParameterValue GetMax() const { return this->max; }
  const ParameterType GetType() const { return this->type; }

 private:
  ParameterValue min;
  ParameterValue max;
  ParameterValue current;
  ParameterType type;

  constexpr ParameterValue clamp(std::int32_t value) {
    std::int32_t min_int32 = static_cast<std::int32_t>(this->min),
                 max_int32 = static_cast<std::int32_t>(this->max);

    if (value < min_int32) {
      return this->min;
    }
    if (value > max_int32) {
      return this->max;
    }
    return value;
  }
};

#endif
