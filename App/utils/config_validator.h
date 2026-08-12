#ifndef CONFIG_VALIDATOR_H
#define CONFIG_VALIDATOR_H

#include "config_table.h"

typedef enum {
    CONFIG_VALIDATOR_RESULT_ERROR = 0,
    CONFIG_VALIDATOR_RESULT_OK,
} ConfigValidatorResult;

typedef struct {
    Value_t *config_table;
    Value_t value_min;
    Value_t value_max;
    Key_t table_count;
    const char *config_table_name;
} ConfigTableValidationSubject;

#define ConfigValidator_REGISTER_CONFIG_TABLE_1D(KEY_TYPE, VALUE_TYPE, TABLE_COUNT, VALUE_MIN,     \
                                                 VALUE_MAX)                                        \
    static ConfigTableValidationSubject CONCATENATE2(ConfigTable_NAME(KEY_TYPE, VALUE_TYPE),       \
                                                     _validation_subject) = {                      \
        .config_table = ConfigTable_NAME(KEY_TYPE, VALUE_TYPE),                                    \
        .table_count = TABLE_COUNT,                                                                \
        .value_min = VALUE_MIN,                                                                    \
        .value_max = VALUE_MAX,                                                                    \
        .config_table_name = VARIABLE_TO_STR(ConfigTable_NAME(KEY_TYPE, VALUE_TYPE))};             \
    __attribute__((constructor, used)) static void CONCATENATE2(                                   \
        ConfigValidator_REGISTER_CONFIG_TABLE__, ConfigTable_NAME(KEY_TYPE, VALUE_TYPE))()         \
    {                                                                                              \
        ConfigValidator_RegisterConfigTableValidationSubject(                                      \
            &CONCATENATE2(ConfigTable_NAME(KEY_TYPE, VALUE_TYPE), _validation_subject));           \
    }

void ConfigValidator_RegisterConfigTableValidationSubject(ConfigTableValidationSubject *subject);

typedef struct {
    ConfigTableValidationSubject *subject;
    uint8_t option_flag;
} ConfigTableKeySetOption;

#define ConfigValidator_REGISTER_CONFIG_TABLE_KEY_SET(KEY_TYPE, VALUE_TYPE, KEY_OPTION)            \
    static ConfigTableKeySetOption CONCATENATE2(ConfigTable_NAME(KEY_TYPE, VALUE_TYPE),            \
                                                _key_set_option) = {};                             \
    __attribute__((constructor, used)) static void CONCATENATE2(                                   \
        ConfigValidator_REGISTER_CONFIG_TABLE_KEY_SET, ConfigTable_NAME(KEY_TYPE, VALUE_TYPE))()   \
    {                                                                                              \
        ConfigValidator_RegisterConfigTableKeySetOption(                                           \
            &CONCATENATE2(ConfigTable_NAME(KEY_TYPE, VALUE_TYPE), _key_set_option));               \
    }
void ConfigValidator_RegisterConfigTableKeySetOption(ConfigTableKeySetOption *option);

ConfigValidatorResult ConfigValidator_Validate();
/**
 * ConfigTable 검사 항목
 * 1. 값 유효 범위 검사(이 검사를 하게 되면 값 범위를 조사하여 필수 등록 값이 등록되어 있는지 확인
 가능)
 * 2. 여러 테이블의 합집합 커버리지
 * - 값이 한 번이라도 키로 사용된 경우
 *      - B.key ∪ C.key ∪ ... = A.value
 * - 값이 한 번이라도 키로 사용되었고, 키로 사용한 테이블이 유일한 경우
 *      - B.key ∪ C.key ∪ ... = A.value
 *      - B.key ∩ C.key ∩ ... = ∅
 * - 값이 한 번이라도 키로 사용되지 않아도 괜찮은 경우
 *      - 이건 KeySetOption을 등록하지 않은 경우에 해당
 */
ConfigValidatorResult ConfigValidator_ValidateConfigTable();

#endif
