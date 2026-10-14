#ifndef DSP_UTILS_HPP
#define DSP_UTILS_HPP

#include "parameter_unsigned_range_value.hpp"

namespace Dsp::Utils {

float GetCutoffFrequency(ParameterModel::UnsignedRangeValue cutoff);

}  // namespace Dsp::Utils

#endif
