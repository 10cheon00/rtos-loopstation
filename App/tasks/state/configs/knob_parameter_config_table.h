#ifndef KNOB_PARAMETER_CONFIG_TABLE_H
#define KNOB_PARAMETER_CONFIG_TABLE_H

#include <stddef.h>

#include "knob_id.h"
#include "parameter_id.h"
#include "enum_map.hpp"

namespace KnobParameterConfig {
    
EnumMap<KnobId, ParameterId> GetEnumMap();

}

#endif