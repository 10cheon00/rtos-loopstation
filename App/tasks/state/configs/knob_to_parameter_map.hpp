#ifndef KNOB_TO_PARAMETER_MAP_HPP
#define KNOB_TO_PARAMETER_MAP_HPP

#include <stddef.h>

#include "knob_id.hpp"
#include "enum_map.hpp"
#include "parameter_id.hpp"

namespace KnobToParameterMap {

ParameterId Get(KnobId id);

}  // namespace KnobToParameterMap

#endif
