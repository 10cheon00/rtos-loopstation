#include "config_validator.h"

#include <stdbool.h>
#include <stddef.h>

#include "config_table.h"

#define CONFIG_TABLE_VALIDATION_SUBJECT_COUNT_MAX 16
#define CONFIG_TABLE_KEY_SET_OPTION_COUNT_MAX 16

static ConfigTableValidationSubject *subjects[CONFIG_TABLE_VALIDATION_SUBJECT_COUNT_MAX];
static size_t subject_count = 0;

static ConfigValidatorResult ValidateConfigTable();
static ConfigValidatorResult
ValidateConfigTableValueValidity(ConfigTableValidationSubject *subject);
static void AddValidationLog();

void ConfigValidator_RegisterConfigTableValidationSubject(ConfigTableValidationSubject *subject)
{
    subjects[subject_count++] = subject;
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
        if (subjects[i]->type == CONFIG_TABLE_TYPE_NO_EMPTY_VALUE) {
            result1 = ValidateConfigTableValueValidity(subjects[i]);
        }
    }
    // TODO:
    // 여러 테이블의 합집합 커버리지 검사
    // enum Id를 모두 사용하지 않는 테이블에 대해서 정보를 등록하는 것으로..?
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
