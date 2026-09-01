#include "parameter.h"

static Parameter_t clamp(Parameter_t min, Parameter_t max, Parameter_t value) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}


void Parameter_AddValue(Parameter *parameter, Parameter_t value) {
    parameter->current = clamp(parameter->min, parameter->max, parameter->current + value);
}

void Parameter_SetValue(Parameter *parameter, Parameter_t value) {
    parameter->current = clamp(parameter->min, parameter->max, value);
}

void Parameter_ToggleValue(Parameter *parameter) {
    if (parameter->current == parameter->max) {
        parameter->current = parameter->min;
    } else {
        parameter->current = parameter->max;
    }
}

