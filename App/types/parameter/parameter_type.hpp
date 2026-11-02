#ifndef PARAMETER_TYPE_HPP
#define PARAMETER_TYPE_HPP

#include <cstdint>

enum class ParameterType {
  NONE = 0,
  TOGGLE,
  UNSIGNED_RANGE,  // 0 ~ 100
  SIGNED_RANGE,    // -50 ~ 50
  DECIMAL_RANGE,   // 0.0 ~ 10.0
  MEASURE, // 음표만
  MEASURE_WITH_UNSIGNED_RANGE, // 음표 + 0 ~ 100
  MEASURE_WITH_MILLISECONDS,
  SELECTED_FX,
  ENUM,
};

#endif
