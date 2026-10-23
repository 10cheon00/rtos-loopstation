#ifndef DSP_UTILS_HPP
#define DSP_UTILS_HPP

#include <cstdint>

#include "parameter_unsigned_range_value.hpp"

namespace Dsp::Utils {

inline constexpr float HZ_MAX = 7000.0f;
inline constexpr float HZ_MIN = 30.0f;
inline constexpr float PI = 3.14159265f;

float GetCutoffFrequency(ParameterModel::UnsignedRangeValue cutoff);

// Numeric Rate only (1~100); out-of-range inputs are clamped.
float GetRateFrequency(std::int16_t numeric_rate);

// Resonance 0~100를 Q로 변환하며, 범위를 벗어난 입력은 제한합니다.
float GetResonanceQ(std::int16_t resonance);

}  // namespace Dsp::Utils

#endif
