#ifndef CONFIG_VALIDATOR_H
#define CONFIG_VALIDATOR_H

#include "config_map.h"

typedef enum {
    CONFIG_VALIDATOR_RESULT_ERROR = 0,
    CONFIG_VALIDATOR_RESULT_OK,
} ConfigValidatorResult;

typedef uint32_t Hash;

Hash ConfigValidator_TypeToHash(const char *type_name);
// #define ConfigValidator_TYPE_TO_HASH(type) ConfigValidator_TypeToHash(#type)

typedef struct {
    ConfigMap *map;
    Hash key_hash;
    Hash value_hash;
} ConfigMapValidationSubject;

ConfigValidatorResult ConfigValidator_AddConfigMap(ConfigMap *map, Hash key_hash, Hash value_hash);

#define MACRO_CONCATENATE_IMPL(A, B) A##B
#define MACRO_CONCATENATE_IMPL2(A, B, C) A##B##C
#define MACRO_CONCATENATE(A, B) MACRO_CONCATENATE_IMPL(A, B)
#define MACRO_CONCATENATE2(A, B, C) MACRO_CONCATENATE_IMPL2(A, B, C)

#define ConfigValidator_REGISTER(map_pointer, KeyType, ValueType)      \
    __attribute__((constructor, used)) static void MACRO_CONCATENATE2(                             \
        ConfigValidator_CreateMapAndRegisterToValidator_, KeyType, ValueType)();                   \
    __attribute__((constructor, used)) static void MACRO_CONCATENATE2(                             \
        ConfigValidator_CreateMapAndRegisterToValidator_, KeyType, ValueType)()                    \
    {                                                                                              \
        ConfigValidator_AddConfigMap(map_pointer, ConfigValidator_TypeToHash(#KeyType),            \
                                     ConfigValidator_TypeToHash(#ValueType));                      \
    }
/**
 * 모든 enum에 대한 엔트리가 맵에 있는지 검사하는 것 (X)
 * 현재 맵의 값 타입을 다른 맵의 키 타입으로 사용할 때, 현재 맵에 존재하는 값이 다른 맵의 키로
 * 사용되지 않는지 검사하는 것 (O)
 * 이를 위해서는 설정 등록 시 키와 값을 저장하여 다른 맵과의 관계 그래프를 만들어야 한다.
 */
ConfigValidatorResult ConfigValidator_Validate();

#endif
