#ifndef PARAMETER_MEASURE_WITH_UNSIGNED_RANGE_VALUE_HPP
#define PARAMETER_MEASURE_WITH_UNSIGNED_RANGE_VALUE_HPP

#include <cstdbool>
#include <cstdint>

#include "fx_measure.hpp"
#include "rtos_parameter_copy.h"

namespace ParameterModel {

/**
 * Measure + UnsignedRangeValue
 * 0 부터 Measure::COUNT까지는 Measure, Measure::COUNT보다 큰 값은
 * UnsignedRangeValue로 취급 current가 인덱스로 동작
 */
struct MeasureWithUnsignedRangeValue {
  std::int16_t measure_offset;
  std::int16_t min;
  std::int16_t max;
  std::int16_t current;

  constexpr MeasureWithUnsignedRangeValue(std::int16_t min, std::int16_t max,
                                          std::int16_t current)
      : measure_offset(static_cast<std::int16_t>(Fx::Measure::WHOLE) -
                       static_cast<std::int16_t>(Fx::Measure::SIXTEENTH)),
        min(min),
        max(max),
        current(current) {}

  constexpr MeasureWithUnsignedRangeValue(
      RtosParameterValue_MeasureWithUnsignedRangeValue rtos_parameter_value)
      : MeasureWithUnsignedRangeValue(rtos_parameter_value.min,
                                      rtos_parameter_value.max,
                                      rtos_parameter_value.current) {}

  std::int16_t clamp(int16_t value) {
    if (value < min) {
      return min;
    }
    if (value > max + measure_offset) {
      return max + measure_offset;
    }
    return value;
  }

  bool IsMeasure() { return current <= min + measure_offset; }

  Fx::Measure ToMeasure() {
    if (!IsMeasure()) {
      return Fx::Measure::SIXTEENTH;
    }
    return static_cast<Fx::Measure>(current);
  }

  std::int16_t ToUnsignedRangeValue() {
    if (IsMeasure()) {
      return 0;
    }
    return current - measure_offset;
  }
};

}  // namespace ParameterModel

#endif
