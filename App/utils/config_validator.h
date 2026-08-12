#ifndef CONFIG_VALIDATOR_H
#define CONFIG_VALIDATOR_H

#include "config_map.h"
#include "config_table.h"

typedef enum {
    CONFIG_VALIDATOR_RESULT_ERROR = 0,
    CONFIG_VALIDATOR_RESULT_OK,
} ConfigValidatorResult;

typedef uint32_t Hash_t;

Hash_t ConfigValidator_TypeToHash(const char *type_name);

typedef struct {
    ConfigMap *map;
    Hash_t key_hash;
    Hash_t value_hash;
    const char *map_name;   // 디버깅 편의를 위한 멤버
    const char *key_name;   // 디버깅 편의를 위한 멤버
    const char *value_name; // 디버깅 편의를 위한 멤버
} ConfigMapValidationSubject;

ConfigValidatorResult ConfigValidator_AddConfigMap(ConfigMap *map, Hash_t key_hash,
                                                   Hash_t value_hash, const char *map_name,
                                                   const char *key_name, const char *value_name);

#define MACRO_CONCATENATE_IMPL(A, B) A##B
#define MACRO_CONCATENATE_IMPL2(A, B, C) A##B##C
#define MACRO_CONCATENATE(A, B) MACRO_CONCATENATE_IMPL(A, B)
#define MACRO_CONCATENATE2(A, B, C) MACRO_CONCATENATE_IMPL2(A, B, C)

#define ConfigValidator_REGISTER(map_pointer, KeyType, ValueType)                                  \
    __attribute__((constructor, used)) static void MACRO_CONCATENATE2(                             \
        ConfigValidator_CreateMapAndRegisterToValidator_, KeyType, ValueType)();                   \
    __attribute__((constructor, used)) static void MACRO_CONCATENATE2(                             \
        ConfigValidator_CreateMapAndRegisterToValidator_, KeyType, ValueType)()                    \
    {                                                                                              \
        ConfigValidator_AddConfigMap(map_pointer, ConfigValidator_TypeToHash(#KeyType),            \
                                     ConfigValidator_TypeToHash(#ValueType), #map_pointer,         \
                                     #KeyType, #ValueType);                                        \
    }

ConfigValidatorResult ConfigValidator_Validate();
/**
 * 검사 항목
 * 1. 필수 등록 항목 검사
 * 2. 값 유효 범위 검사
 * 3. UiActionId와 TrackActionId의 유효한 값을 갖는 키들이 서로 배타관계에 있는지, 그리고 두 집합의
 * 합집합이 ButtonId인지 검사
 */
ConfigValidatorResult ConfigValidator_ValidateConfigTable();

#endif
