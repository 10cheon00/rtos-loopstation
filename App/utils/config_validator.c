#include "config_validator.h"

#include <stdbool.h>

#include "config_map.h"

#define CONFIG_MAP_VALIDATION_NODE_COUNT 16

typedef struct ConfigMapTreeNode ConfigMapTreeNode;

struct ConfigMapTreeNode {
    // 이 맵의 키 타입을 값으로 쓰는 부모 노드.
    ConfigMapTreeNode *parent;
    ConfigMapValidationSubject *subject;
    bool coverage_checked;
};

typedef struct {
    ConfigMapTreeNode nodes[CONFIG_MAP_VALIDATION_NODE_COUNT];
    size_t node_count;
} ConfigMapTree;

typedef struct {
    Value_t value;                       // 맵에 존재하지 않는 키
    ConfigMapValidationSubject *subject; // value를 키로 갖지 않는 ConfigMap
} ConfigValidatorLog;

// 검증 목록
static ConfigMapValidationSubject config_map_validation_subjects[CONFIG_MAP_VALIDATION_NODE_COUNT];
static size_t config_map_validation_subject_count = 0;

// 검증 트리
static ConfigMapTree tree;

// 로그
#define LOG_COUNT 128
static ConfigValidatorLog logs[LOG_COUNT];
static size_t log_count = 0;

static void InitConfigMapTree();
static void TopologySort();
static ConfigValidatorResult Validate();
static void ValidateKeyCoverage(ConfigMapTreeNode *node);
static void AddConfigValidatorLog(ConfigMapValidationSubject *subject, Value_t value);

Hash_t ConfigValidator_TypeToHash(const char *type_name)
{
    return djb2(type_name);
}

ConfigValidatorResult ConfigValidator_AddConfigMap(ConfigMap *map, Hash_t key_hash,
                                                   Hash_t value_hash, const char *map_name,
                                                   const char *key_name, const char *value_name)
{
    if (config_map_validation_subject_count == CONFIG_MAP_VALIDATION_NODE_COUNT) {
        return CONFIG_VALIDATOR_RESULT_ERROR;
    }
    config_map_validation_subjects[config_map_validation_subject_count++] =
        (ConfigMapValidationSubject){.map = map,
                                     .key_hash = key_hash,
                                     .value_hash = value_hash,
                                     .map_name = map_name,
                                     .key_name = key_name,
                                     .value_name = value_name};
    return CONFIG_VALIDATOR_RESULT_OK;
}

ConfigValidatorResult ConfigValidator_Validate()
{
    // 1. 트리 생성
    // 2. 생성된 트리의 깊이를 따져 가장 낮은 깊이를 갖는 노드부터 탐색
    // 3. 한 맵(A)의 값 타입을 키로 사용하는 다른 맵(B)과 관계를 본다.
    //    A의 모든 값이 B의 모든 엔트리에 키로 존재하는지 확인한다.

    InitConfigMapTree();
    TopologySort();
    return Validate();
}

static void InitConfigMapTree()
{
    tree.node_count = 0;
    log_count = 0;

    // Node 생성
    // 노드는 위상 정렬에 필요하며, 탐색을 시작할 노드들을 선택하기위해 필요하다.
    // 이 때 각 Node는 validation_subject와 1대1로 대응된다. 예를 들어 3번째 Node는 3번쨰
    // validation_subject다.
    for (size_t i = 0; i < config_map_validation_subject_count; i++) {
        tree.nodes[tree.node_count++] =
            (ConfigMapTreeNode){.parent = NULL,
                                .subject = &config_map_validation_subjects[i],
                                .coverage_checked = false};
    }
}

static void TopologySort()
{
    // 간선은 한 맵의 값 타입이 다른 맵의 키 타입으로 쓰일 때 생기므로, 간선 탐색은 O(n^2)의
    // 시간복잡도로 수행된다. 간선을 모두 저장하기에는 어려우므로, 값 타입으로 쓰는 맵을 부모 노드로
    // 저장만 한다.

    for (size_t i = 0; i < config_map_validation_subject_count; i++) {
        for (size_t j = 0; j < config_map_validation_subject_count; j++) {
            if (i == j) {
                continue;
            }
            Hash_t map_a_value_hash = config_map_validation_subjects[i].value_hash;
            Hash_t map_b_key_hash = config_map_validation_subjects[j].key_hash;
            if (map_a_value_hash == map_b_key_hash) {
                tree.nodes[j].parent = &tree.nodes[i];
            }
        }
    }
}

/**
 * 한 맵의 모든 값이 한 번이라도 다른 맵의 키로 사용되었는지 검사한다.
 *
 */
static ConfigValidatorResult Validate()
{
    // 커버리지 검사를 수행하지 않은 모든 노드에 대해 커버리지 검사를 수행
    for (size_t i = 0; i < tree.node_count; i++) {
        ValidateKeyCoverage(&tree.nodes[i]);
    }
    if (log_count > 0) {
        return CONFIG_VALIDATOR_RESULT_ERROR;
    }
    return CONFIG_VALIDATOR_RESULT_OK;
}

static void ValidateKeyCoverage(ConfigMapTreeNode *node)
{
    ConfigValidatorResult result = CONFIG_VALIDATOR_RESULT_OK;
}

static void AddConfigValidatorLog(ConfigMapValidationSubject *subject, Value_t value)
{
    logs[log_count++] = (ConfigValidatorLog){.subject = subject, .value = value};
}
