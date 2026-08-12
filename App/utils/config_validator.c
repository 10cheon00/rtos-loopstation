#include "config_validator.h"

#include <stdbool.h>
#include <stddef.h>

#include "config_table.h"

#define CONFIG_TABLE_VALIDATION_SUBJECT_COUNT_MAX 16
#define CONFIG_TABLE_KEY_SET_OPTION_COUNT_MAX 16

static ConfigTableValidationSubject *subjects[CONFIG_TABLE_VALIDATION_SUBJECT_COUNT_MAX];
static size_t subject_count = 0;
static ConfigTableKeySetOption *key_set_options[CONFIG_TABLE_KEY_SET_OPTION_COUNT_MAX];
static size_t key_set_option_count = 0;

static ConfigValidatorResult ValidateConfigTable();
static ConfigValidatorResult
ValidateConfigTableValueValidity(ConfigTableValidationSubject *subject);
static void AddValidationLog();

void ConfigValidator_RegisterConfigTableValidationSubject(ConfigTableValidationSubject *subject)
{
    subjects[subject_count++] = subject;
}

void ConfigValidator_RegisterConfigTableKeySetOption(ConfigTableKeySetOption *key_set_option)
{
    key_set_options[key_set_option_count++] = key_set_option;
}

ConfigValidatorResult ConfigValidator_Validate()
{
    ConfigValidatorResult result;
    result = ValidateConfigTable();
    return result;
}
static ConfigValidatorResult ValidateConfigTable()
{
    ConfigValidatorResult result1, result2;
    for (size_t i = 0; i < subject_count; i++) {
        result1 = ValidateConfigTableValueValidity(subjects[i]);
    }
    // TODO: 
    // 
    return result1 == CONFIG_VALIDATOR_RESULT_OK && result2 == CONFIG_VALIDATOR_RESULT_OK;
}

static ConfigValidatorResult ValidateConfigTableValueValidity(ConfigTableValidationSubject *subject)
{
    ConfigValidatorResult result = CONFIG_VALIDATOR_RESULT_OK;
    Value_t value;
    for (Key_t i = 0; i < subject->table_count; i++) {
        value = subject->config_table[i];
        if (value < subject->value_min || value > subject->value_max) {
            result = CONFIG_VALIDATOR_RESULT_ERROR;
            AddValidationLog();
        }
    }
    return result;
}

static void AddValidationLog() {}
