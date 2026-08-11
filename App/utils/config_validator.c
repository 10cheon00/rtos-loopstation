#include "config_validator.h"

#include <stdbool.h>

#include "config_map.h"

#define CONFIG_MAP_VALIDATION_NODE_COUNT 16

typedef struct ConfigMapTreeNode ConfigMapTreeNode;

struct ConfigMapTreeNode {
    // 이 맵의 키 타입을 값으로 쓰는 부모 노드.
    ConfigMapTreeNode *parent;
    ConfigMapValidationSubject *subject;
#define CONFIG_MAP_GRAPH_NODE_DEGREE_MAX SIZE_MAX
    size_t depth; // 이 값이 UINT32_MAX면 깊이가 계산되지 않은 것임
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

// 검증에 사용할 스택 자료구조
#define STACK_CAPACITY (CONFIG_MAP_VALIDATION_NODE_COUNT + 1)
typedef struct {
    ConfigMapTreeNode *array[STACK_CAPACITY];
    size_t top;
} Stack;

static Stack key_coverage_stack;

static void InitStack(Stack *stack);
static void PushStack(Stack *stack, ConfigMapTreeNode *node);
static ConfigMapTreeNode *PopStack(Stack *stack);
static bool IsStackFull(Stack *stack);
static bool IsStackEmpty(Stack *stack);

static void InitConfigMapTree();
static void TopologySort();
static ConfigValidatorResult Validate();
static ConfigValidatorResult ValidateKeyCoverage(ConfigMapTreeNode *node);
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

void InitStack(Stack *stack)
{
    stack->top = 0;
}

void PushStack(Stack *stack, ConfigMapTreeNode *node)
{
    if (IsStackFull(stack)) {
        return;
    }
    stack->array[stack->top++] = node;
}

ConfigMapTreeNode *PopStack(Stack *stack)
{
    if (IsStackEmpty(stack)) {
        return NULL;
    }
    return stack->array[--stack->top];
}

bool IsStackFull(Stack *stack)
{
    return stack->top == STACK_CAPACITY;
}

bool IsStackEmpty(Stack *stack)
{
    return stack->top == 0;
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
            (ConfigMapTreeNode){.depth = 0,
                                .parent = NULL,
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

static ConfigValidatorResult Validate()
{
    ConfigValidatorResult result = CONFIG_VALIDATOR_RESULT_OK;

    // 커버리지 검사를 수행하지 않은 모든 노드에 대해 커버리지 검사를 수행
    for (size_t i = 0; i < tree.node_count; i++) {
        if (tree.nodes[i].parent == NULL) {
            tree.nodes[i].coverage_checked = true;
            continue;
        }
        if (tree.nodes[i].coverage_checked) {
            continue;
        }
        if (ValidateKeyCoverage(&tree.nodes[i]) == CONFIG_VALIDATOR_RESULT_ERROR) {
            result = CONFIG_VALIDATOR_RESULT_ERROR;
        }
    }
    return result;
}

/**
 * 현재 노드에서 루트 노드까지 이동하며 커버리지 검사를 한 노드를 만나거나, 루트 노드에 도달할
 * 때까지 스택에 담는다. 스택에서 꺼낸 노드와 노드의 부모 간 커버리지 검사를 수행한다. 수행 후에는
 * 노드에 커버리지 검사를 수행했음을 표시한다.
 */
ConfigValidatorResult ValidateKeyCoverage(ConfigMapTreeNode *node)
{
    ConfigValidatorResult result = CONFIG_VALIDATOR_RESULT_OK;
    ConfigMapTreeNode *parent, *child;
    ConfigMapTreeNode *iter = node;
    ConfigMap *parent_map, *child_map;
    Value_t value;
    
    InitStack(&key_coverage_stack);
    while (iter->parent != NULL && !iter->coverage_checked) {
        PushStack(&key_coverage_stack, iter);
        iter = iter->parent;
    }
    while (!IsStackEmpty(&key_coverage_stack)) {
        child = PopStack(&key_coverage_stack);
        parent = child->parent;
        parent_map = parent->subject->map;
        child_map = child->subject->map;
        for (size_t i = 0; i < parent_map->count; i++) {
            value = parent_map->entries[i].value;
            if (ConfigMap_Contains(child_map, (Key_t)value) != CONFIG_MAP_RESULT_OK) {
                result = CONFIG_VALIDATOR_RESULT_ERROR;
                AddConfigValidatorLog(child->subject, value);
            }
        }
        child->coverage_checked = true;
    }

    return result;
}

static void AddConfigValidatorLog(ConfigMapValidationSubject *subject, Value_t value)
{
    logs[log_count++] = (ConfigValidatorLog){.subject = subject, .value = value};
}
