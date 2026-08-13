#include "config_validator.h"

#include <stdbool.h>
#include <stddef.h>

#include "id_enum.h"
#include "config_table.h"

#define CONFIG_TABLE_VALIDATION_SUBJECT_COUNT_MAX 16
#define CONFIG_TABLE_COVERAGE_SUBJECT_COUNT_MAX 16
#define CONFIG_VALIDATOR_LOG_COUNT_MAX 128

typedef ConfigTableValidationSubject *ConfigTableValidationSubjectPointer;

static ConfigTableValidationSubjectPointer
    table_subject_pointers[CONFIG_TABLE_VALIDATION_SUBJECT_COUNT_MAX];
static size_t table_subject_pointer_count = 0;

static ConfigTableCoverageValidationSubject
    coverage_subjects[CONFIG_TABLE_VALIDATION_SUBJECT_COUNT_MAX];
static size_t coverage_subject_count = 0;

static ConfigValidatorLog logs[CONFIG_VALIDATOR_LOG_COUNT_MAX];
static size_t log_count;

static ConfigValidatorResult ValidateConfigTable();
static void ValidateConfigTableValueValidity(ConfigTableValidationSubject *subject);
static void FindConfigTableCoverageValidationSubjects();

static void AddCoverageValidationSubject(ConfigTableValidationSubject *source_subject,
                                         ConfigTableValidationSubject *target);
static void ValidateConfigTableCoverageValidity();
static void ValidateAllTargetTableKeyInSourceTable(ConfigTableValidationSubject *source,
                                                   ConfigTableValidationSubject *target);
static void AddValidationLog();
static bool IsValidValue(ConfigTableValidationSubject *subject, Value_t value);

void ConfigValidator_RegisterConfigTableValidationSubject(ConfigTableValidationSubject *subject)
{
    table_subject_pointers[table_subject_pointer_count++] = subject;
}

ConfigValidatorResult ConfigValidator_Validate()
{
    ConfigValidatorResult result;
    result = ValidateConfigTable();
    return result;
}

/**
 * ConfigTable 검사 항목
 * 1. 값 유효 범위 검사(이 검사를 하게 되면 값 범위를 조사하여 필수 등록 값이 등록되어
 있는지 확인 가능)
 * 2. 여러 테이블의 합집합 커버리지
 *  값이 한 번이라도 키로 사용된 경우를 조사
 *      - B.key ∪ C.key ∪ ... = A.value
 */
static ConfigValidatorResult ValidateConfigTable()
{
    for (size_t i = 0; i < table_subject_pointer_count; i++) {
        ValidateConfigTableValueValidity(table_subject_pointers[i]);
    }

    FindConfigTableCoverageValidationSubjects();
    ValidateConfigTableCoverageValidity();

    return log_count > 0 ? CONFIG_VALIDATOR_RESULT_ERROR : CONFIG_VALIDATOR_RESULT_OK;
}

static void ValidateConfigTableValueValidity(ConfigTableValidationSubject *subject)
{
    Value_t value;
    for (Key_t i = 0; i < subject->table_count; i++) {
        value = subject->config_table[i];
        // TODO:
        // 테이블 정책에 따라 다르게 검사해야하는데, NULL값을 허용하는지 이미 조건으로 주지 않나..?
        if (table_subject_pointers[i]->type == CONFIG_TABLE_TYPE_NO_NULL_VALUE) {
            if (!IsValidValue(subject, value)) {
                // TODO:
                //  로그남기기
                AddValidationLog();
            }
        } else {
            if (value != ID_NULL && !IsValidValue(subject, value)) {
                AddValidationLog();
            }
        }
    }
}

static void FindConfigTableCoverageValidationSubjects()
{
    for (size_t i = 0; i < table_subject_pointer_count; i++) {
        for (size_t j = 0; j < table_subject_pointer_count; j++) {
            if (i == j) {
                continue;
            }
            if (table_subject_pointers[i]->value_hash == table_subject_pointers[j]->key_hash) {
                AddCoverageValidationSubject(table_subject_pointers[i], table_subject_pointers[j]);
            }
        }
    }
}

static void AddCoverageValidationSubject(ConfigTableValidationSubject *source,
                                         ConfigTableValidationSubject *target)
{
    coverage_subjects[coverage_subject_count++] = (ConfigTableCoverageValidationSubject){
        .source = source,
        .target = target,
    };
}

static void ValidateConfigTableCoverageValidity()
{
    for (size_t i = 0; i < coverage_subject_count; i++) {
        ValidateAllTargetTableKeyInSourceTable(coverage_subjects[i].source,
                                               coverage_subjects[i].target);
    }
}

static void ValidateAllTargetTableKeyInSourceTable(ConfigTableValidationSubject *source,
                                                   ConfigTableValidationSubject *target)
{
    bool is_equal;
    for (Key_t target_key = 0; target_key < target->table_count; target_key++) {
        if (IsValidValue(target, target_key)) {
            is_equal = false;
            for (Key_t source_key = ID_NULL; source_key < source->table_count; source_key++) {
                if (!IsValidValue(source, source_key)) {
                    continue;
                }
                if (source->config_table[source_key] == target_key) {
                    is_equal = true;
                    break;
                }
            }
            if (!is_equal) {
                // TODO:
                //  로그남기기
                AddValidationLog();
            }
        }
    }
}

static void AddValidationLog() {}

static bool IsValidValue(ConfigTableValidationSubject *subject, Value_t value)
{
    return subject->value_min <= value && subject->value_max >= value;
}
