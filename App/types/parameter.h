#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdint.h>

typedef int8_t Parameter_t;

typedef struct {
    Parameter_t min;
    Parameter_t max;
    Parameter_t current;
} Parameter;

void Parameter_AddValue(Parameter* parameter, Parameter_t value);
void Parameter_SetValue(Parameter* parameter, Parameter_t value);

#endif
