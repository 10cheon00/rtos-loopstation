#ifndef CONFIG_VALIDATOR_H
#define CONFIG_VALIDATOR_H

#include "config_table.h"

typedef enum {
    CONFIG_VALIDATOR_RESULT_ERROR = 0,
    CONFIG_VALIDATOR_RESULT_OK,
} ConfigValidatorResult;

ConfigValidatorResult ConfigValidator_Validate();
/**
 * ConfigTable 검사 항목
 * 1. 필수 등록 항목 검사
 * 2. 값 유효 범위 검사
 * 3. UiActionId와 TrackActionId의 유효한 값을 갖는 키들이 서로 배타관계에 있는지, 그리고 두 집합의
 * 합집합이 ButtonId인지 검사 --> 얘는 그래프를 만들어야 할듯
 */
ConfigValidatorResult ConfigValidator_ValidateConfigTable();

#endif
