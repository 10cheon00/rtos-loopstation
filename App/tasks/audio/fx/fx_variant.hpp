#ifndef FX_VARIANT_HPP
#define FX_VARIANT_HPP

#include <variant>

#include "parameter.hpp"
#include "parameter_value.hpp"

namespace Fx {

using namespace ParameterModel;

struct Lpf {
  UnsignedRangeValue rate{0, 100, 100}; // TODO: 음표와 숫자를 혼합한 타입으로 변경해야함
  UnsignedRangeValue depth{0, 100, 50};
  UnsignedRangeValue resonance{0, 100, 50};
  UnsignedRangeValue cutoff{0, 100, 50};
};

using Variant = std::variant<std::monostate, Lpf>;

}  // namespace Fx

#endif
