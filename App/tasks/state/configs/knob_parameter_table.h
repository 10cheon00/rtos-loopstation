#ifndef KNOB_PARAMETER_TABLE_H
#define KNOB_PARAMETER_TABLE_H

#include <stddef.h>

#include "knob_id.h"
#include "parameter_id.h"

ParameterId KnobParameterTable_GetParameterIdFromKnobId(KnobId knob_id);

#endif