#include "config_validator.h"

#include <stdbool.h>
#include <stddef.h>

#include "config_table.h"
#include "id.h"

#define CONFIG_TABLE_VALIDATION_SUBJECT_COUNT_MAX 16
#define CONFIG_TABLE_COVERAGE_SUBJECT_COUNT_MAX 16
#define CONFIG_VALIDATOR_LOG_COUNT_MAX 128

typedef ConfigTableValidationSubject* ConfigTableValidationSubjectPointer;

static ConfigTableValidationSubjectPointer
    table_subject_pointers[CONFIG_TABLE_VALIDATION_SUBJECT_COUNT_MAX];
static size_t table_subject_pointer_count = 0;

static ConfigTableCoverageValidationSubject
    coverage_subjects[CONFIG_TABLE_VALIDATION_SUBJECT_COUNT_MAX];
static size_t coverage_subject_count = 0;

static ConfigValidatorLog logs[CONFIG_VALIDATOR_LOG_COUNT_MAX];
static size_t log_count;

static ConfigValidatorResult ValidateConfigTable();
static void ValidateConfigTableValueValidity(
    ConfigTableValidationSubject* subject);
static void FindConfigTableCoverageValidationSubjects();

static void AddCoverageValidationSubject(
    ConfigTableValidationSubject* source_subject,
    ConfigTableValidationSubject* target);
static void ValidateConfigTableCoverageValidity();
static void ValidateAllTargetTableKeyInSourceTable(
    ConfigTableValidationSubject* source, ConfigTableValidationSubject* target);
static void ValidateAllSourceValueInTargetSubjects(
    ConfigTableValidationSubject* source);
static void AddValidationLog(ConfigTableValidationSubject* subject,
                             ConfigValidatorLogType type, Key_t key,
                             Value_t value);
static bool IsValidValue(ConfigTableValidationSubject* subject, Value_t value);

void ConfigValidator_RegisterConfigTableValidationSubject(
    ConfigTableValidationSubject* subject) {
  table_subject_pointers[table_subject_pointer_count++] = subject;
}

ConfigValidatorResult ConfigValidator_Validate() {
  ConfigValidatorResult result;
  result = ValidateConfigTable();
  return result;
}

ConfigValidatorLog* ConfigValidator_GetLog() { return logs; }

/**
 * ConfigTable 검사 항목
 * 1. 값 유효 범위 검사(이 검사를 하게 되면 값 범위를 조사하여 필수 등록 값이
 등록되어 있는지 확인 가능)
 * 2. 여러 테이블의 합집합 커버리지
 *  값이 한 번이라도 키로 사용된 경우를 조사
 *      - B.key ∪ C.key ∪ ... = A.value
 */
static ConfigValidatorResult ValidateConfigTable() {
  for (size_t i = 0; i < table_subject_pointer_count; i++) {
    ValidateConfigTableValueValidity(table_subject_pointers[i]);
  }

  FindConfigTableCoverageValidationSubjects();
  ValidateConfigTableCoverageValidity();

  return log_count > 0 ? CONFIG_VALIDATOR_RESULT_ERROR
                       : CONFIG_VALIDATOR_RESULT_OK;
}

static void ValidateConfigTableValueValidity(
    ConfigTableValidationSubject* subject) {
  Value_t value;
  for (Key_t key = ID_NONE + 1; key < subject->table_count; key++) {
    value = subject->GetValue(key);
    if (!IsValidValue(subject, value)) {
      AddValidationLog(subject, CONFIG_VALIDATOR_LOG_TYPE_INVALID_VALUE, 0,
                       value);
    }
  }
}

static void FindConfigTableCoverageValidationSubjects() {
  for (size_t i = 0; i < table_subject_pointer_count; i++) {
    for (size_t j = 0; j < table_subject_pointer_count; j++) {
      if (i == j) {
        continue;
      }
      if (table_subject_pointers[i]->value_hash ==
          table_subject_pointers[j]->key_hash) {
        AddCoverageValidationSubject(table_subject_pointers[i],
                                     table_subject_pointers[j]);
      }
    }
  }
}

static void AddCoverageValidationSubject(ConfigTableValidationSubject* source,
                                         ConfigTableValidationSubject* target) {
  coverage_subjects[coverage_subject_count++] =
      (ConfigTableCoverageValidationSubject){
          .source = source,
          .target = target,
      };
}

static void ValidateConfigTableCoverageValidity() {
  bool already_checked;
  for (size_t i = 0; i < coverage_subject_count; i++) {
    ValidateAllTargetTableKeyInSourceTable(coverage_subjects[i].source,
                                           coverage_subjects[i].target);
  }
  for (size_t i = 0; i < coverage_subject_count; i++) {
    // i보다 이전 항목들은 이미 검사를 완료한 source이므로 건너뛰기
    already_checked = false;
    for (size_t j = 0; j < i; j++) {
      if (coverage_subjects[i].source == coverage_subjects[j].source) {
        already_checked = true;
        break;
      }
    }
    if (already_checked) {
      continue;
    }
    ValidateAllSourceValueInTargetSubjects(coverage_subjects[i].source);
  }
}

static void ValidateAllTargetTableKeyInSourceTable(
    ConfigTableValidationSubject* source,
    ConfigTableValidationSubject* target) {
  bool is_equal;
  for (Key_t target_key = ID_NONE + 1; target_key < target->table_count;
       target_key++) {
    if (IsValidValue(target, target_key)) {
      is_equal = false;
      for (Key_t source_key = ID_NONE + 1; source_key < source->table_count;
           source_key++) {
        if (!IsValidValue(source, source_key)) {
          continue;
        }
        if (source->GetValue(source_key) == target_key) {
          is_equal = true;
          break;
        }
      }
      if (!is_equal) {
        AddValidationLog(
            target, CONFIG_VALIDATOR_LOG_TYPE_NO_TARGET_KEY_IN_SOURCE_VALUES,
            target_key, 0);
      }
    }
  }
}

static void ValidateAllSourceValueInTargetSubjects(
    ConfigTableValidationSubject* source) {
  Key_t target_key;
  Value_t source_value;
  ConfigTableValidationSubject* target;
  for (Key_t key = 0; key < source->table_count; key++) {
    source_value = source->GetValue(key);
    if (!IsValidValue(source, source_value)) {
      continue;
    }
    for (size_t i = 0; i < coverage_subject_count; i++) {
      target = coverage_subjects[i].target;
      if (coverage_subjects[i].source == source) {
        // value가 key로 존재하는건지 확인
        target_key = (Key_t)source_value;
        if (!IsValidValue(target, target->GetValue(target_key))) {
          AddValidationLog(
              source,
              CONFIG_VALIDATOR_LOG_TYPE_NO_SOURCE_VALUE_IN_ANY_TARGET_KEYS, 0,
              source_value);
        }
      }
    }
  }
}

static void AddValidationLog(ConfigTableValidationSubject* subject,
                             ConfigValidatorLogType type, Key_t key,
                             Value_t value) {
  logs[log_count].subject = subject;
  logs[log_count].type = type;
  if (type == CONFIG_VALIDATOR_LOG_TYPE_NO_SOURCE_VALUE_IN_ANY_TARGET_KEYS) {
    logs[log_count].key_or_value.value = value;
  } else if (type == CONFIG_VALIDATOR_LOG_TYPE_NO_TARGET_KEY_IN_SOURCE_VALUES) {
    logs[log_count].key_or_value.key = key;
  } else {
    logs[log_count].key_or_value.value = value;
  }
  log_count++;
}

static bool IsValidValue(ConfigTableValidationSubject* subject, Value_t value) {
  if (subject->type == CONFIG_TABLE_TYPE_ALLOW_NULL_VALUE) {
    return value == ID_NULL ||
           subject->value_min <= value && subject->value_max >= value;
  }
  return subject->value_min <= value && subject->value_max >= value;
}
