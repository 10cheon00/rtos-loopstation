#ifndef PARAMETER_RAW_H
#define PARAMETER_RAW_H

#include <stdbool.h>
#include <stdint.h>

#include "rtos_enum_value.h"

typedef int16_t RtosParameterValue;

typedef struct {
  RtosParameterValue enum_min;
  RtosParameterValue enum_value;
  RtosParameterValue enum_max;
  const char** enum_name_table;
} RtosParameterValue_EnumValue;

typedef struct {
  bool is_on;
} RtosParameterValue_ToggleValue;

typedef struct {
  RtosParameterValue min;
  RtosParameterValue max;
  RtosParameterValue current;
} RtosParameterValue_UnsignedRangeValue;

typedef union {
  RtosParameterValue_UnsignedRangeValue rtos_parameter_unsigned_range_value;
  RtosParameterValue_ToggleValue rtos_parameter_toggle_value;
  RtosParameterValue_EnumValue rtos_parameter_enum_value;
} RtosParameterUnion;

typedef struct {
  RtosEnumValue rtos_enum_value_parameter_type;
  RtosParameterUnion rtos_parameter_union;
} RtosParameterCopy;

#endif
