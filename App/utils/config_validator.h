#ifndef CONFIG_VALIDATOR_H
#define CONFIG_VALIDATOR_H

#include "config_table.h"

typedef enum {
  CONFIG_VALIDATOR_RESULT_ERROR = 0,
  CONFIG_VALIDATOR_RESULT_OK,
} ConfigValidatorResult;

typedef enum {
  CONFIG_TABLE_TYPE_NO_NULL_VALUE,
  CONFIG_TABLE_TYPE_ALLOW_NULL_VALUE,
} ConfigTableType;

typedef Value_t (*ConfigTableValueGetter)(Key_t key);

typedef struct {
  ConfigTableValueGetter GetValue;
  Value_t value_min;
  Value_t value_max;
  Key_t table_count;
  ConfigTableType type;
  const char* config_table_name;
  Hash_t key_hash;
  Hash_t value_hash;
} ConfigTableValidationSubject;

// 테이블과 테이블 간 커버리지 검사를 할 때, 값이 한 번이라도 키로 사용된 경우와
// 모든 키가 값으로 존재하는지 검사
typedef struct {
  ConfigTableValidationSubject* source;
  ConfigTableValidationSubject* target;
} ConfigTableCoverageValidationSubject;

#define ConfigValidator_REGISTER_CONFIG_TABLE_1D(KEY_TYPE, VALUE_TYPE,         \
                                                 TABLE_COUNT, VALUE_MIN,       \
                                                 VALUE_MAX, CONFIG_TABLE_TYPE) \
  static Value_t CONCATENATE2(ConfigTable_NAME(KEY_TYPE, VALUE_TYPE),          \
                              _get_value)(Key_t key) {                         \
    return ConfigTable_ToValue(ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[key]);   \
  }                                                                            \
  static ConfigTableValidationSubject CONCATENATE2(                            \
      ConfigTable_NAME(KEY_TYPE, VALUE_TYPE), _validation_subject) = {         \
      .GetValue =                                                              \
          CONCATENATE2(ConfigTable_NAME(KEY_TYPE, VALUE_TYPE), _get_value),    \
      .value_min = VALUE_MIN,                                                  \
      .value_max = VALUE_MAX,                                                  \
      .table_count = TABLE_COUNT,                                              \
      .type = CONFIG_TABLE_TYPE,                                               \
      .config_table_name =                                                     \
          VARIABLE_TO_STR(ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)),             \
  };                                                                           \
  __attribute__((constructor, used)) static void CONCATENATE2(                 \
      ConfigValidator_REGISTER_CONFIG_TABLE__,                                 \
      ConfigTable_NAME(KEY_TYPE, VALUE_TYPE))() {                              \
    ConfigTableValidationSubject* subject = &CONCATENATE2(                     \
        ConfigTable_NAME(KEY_TYPE, VALUE_TYPE), _validation_subject);          \
    subject->key_hash = djb2(VARIABLE_TO_STR(KEY_TYPE));                       \
    subject->value_hash = djb2(VARIABLE_TO_STR(VALUE_TYPE));                   \
    ConfigValidator_RegisterConfigTableValidationSubject(subject);             \
  }

void ConfigValidator_RegisterConfigTableValidationSubject(
    ConfigTableValidationSubject* subject);

ConfigValidatorResult ConfigValidator_Validate();

typedef enum {
  CONFIG_VALIDATOR_LOG_TYPE_NONE = 0,
  CONFIG_VALIDATOR_LOG_TYPE_INVALID_VALUE,
  CONFIG_VALIDATOR_LOG_TYPE_NO_TARGET_KEY_IN_SOURCE_VALUES,
  CONFIG_VALIDATOR_LOG_TYPE_NO_SOURCE_VALUE_IN_ANY_TARGET_KEYS
} ConfigValidatorLogType;

typedef struct {
  ConfigTableValidationSubject* subject;
  ConfigValidatorLogType type;
  union {
    Key_t key;
    Value_t value;
  } key_or_value;
} ConfigValidatorLog;

ConfigValidatorLog* ConfigValidator_GetLog();
#endif
