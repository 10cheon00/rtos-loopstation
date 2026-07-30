#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdint.h>

typedef int8_t Parameter_t;
typedef uint8_t ParameterType_t;

enum {
    PARAMETER_TYPE_NONE = 0,
    PARAMETER_TYPE_SLIDER,
    PARAMETER_TYPE_TOGGLE,
};

typedef struct {
    Parameter_t min;
    Parameter_t max;
    Parameter_t current;
    ParameterType_t type;
} Parameter;

void Parameter_AddValue(Parameter* parameter, Parameter_t value);
void Parameter_SetValue(Parameter* parameter, Parameter_t value);
void Parameter_ToggleValue(Parameter* parameter);

#endif
