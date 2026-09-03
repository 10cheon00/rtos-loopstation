#ifndef KNOB_TO_PARAMETER_MAP_HPP
#define KNOB_TO_PARAMETER_MAP_HPP

#include <stddef.h>

#include "knob_id.h"
#include "enum_map.hpp"
#include "parameter_id.h"

namespace KnobToParameterMap {

ParameterId Get(KnobId id);

}  // namespace KnobToParameterMap

#endif
