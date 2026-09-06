#ifndef PARAMETER_RAW_H
#define PARAMETER_RAW_H

#include <stdint.h>

#include "enum_raw.h"

typedef int8_t ParameterValueRaw;

typedef struct {
  ParameterValueRaw min;
  ParameterValueRaw max;
  ParameterValueRaw current;
  EnumRaw parameter_type_raw;
} ParameterRaw;

#endif
