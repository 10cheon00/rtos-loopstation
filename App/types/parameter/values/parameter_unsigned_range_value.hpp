#ifndef PARAMETER_SLIDER_VALUE_HPP
#define PARAMETER_SLIDER_VALUE_HPP

#include <cstdint>

namespace ParameterModel {

struct UnsignedRangeValue {
  std::int16_t min;
  std::int16_t max;
  std::int16_t current;

  constexpr UnsignedRangeValue(std::int16_t min, std::int16_t max, std::int16_t current)
      : min(min), max(max), current(current) {}

  std::int16_t clamp(int16_t amount) {
    if (amount < min) {
      return min;
    }
    if (amount > max) {
      return max;
    }
    return amount;
  }
};

}  // namespace ParameterModel

#endif
