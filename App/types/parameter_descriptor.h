#ifndef PARAMETER_DESCRIPTOR_H
#define PARAMETER_DESCRIPTOR_H

#include "parameter_id.h"

typedef struct {
    ParameterId id;
    const char *label;
} ParameterDescriptor;

#endif
