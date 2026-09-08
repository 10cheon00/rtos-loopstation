#ifndef PARAMETER_RAW_H
#define PARAMETER_RAW_H

#include <stdint.h>

#include "rtos_enum_value.h"

typedef int8_t RtosParameterValue;

typedef struct {
  RtosParameterValue min;
  RtosParameterValue max;
  RtosParameterValue current;
  RtosEnumValue parameter_type_raw;
} RtosParameterCopy;

#endif
