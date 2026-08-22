#ifndef PARAMETER_SLOT_H
#define PARAMETER_SLOT_H

#include "parameter_id.h"

typedef struct {
    ParameterId id;
    const char *label;
} ParameterSlotConfig;

#endif
