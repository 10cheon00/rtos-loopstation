#include "config_validator.h"

#include <stdbool.h>

#include "config_table.h"

void ConfigValidator_RegisterConfigTable1D() {}

ConfigValidatorResult ConfigValidator_Validate()
{
    ConfigValidatorResult result;
    result = ConfigValidator_ValidateConfigTable();
    return result;
}

ConfigValidatorResult ConfigValidator_ValidateConfigTable()
{
    return CONFIG_VALIDATOR_RESULT_ERROR;
}
