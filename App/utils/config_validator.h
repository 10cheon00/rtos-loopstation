#ifndef CONFIG_VALIDATOR_H
#define CONFIG_VALIDATOR_H

#include "config_map.h"

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
/**
 * 검사 항목
 * 1. 테이블에 값이 있는지 검사
 * 2. 
 */
ConfigValidatorResult ConfigValidator_Validate();

#endif
