#include "config_validator.h"

#include <stdbool.h>

#include "config_map.h"

#define CONFIG_MAP_VALIDATION_NODE_COUNT 16
// 비 사이클 그래프에서 한 노드로 향하는 간선의 수는 1개이므로 간선의 수는 노드 수 - 1임
#define CONFIG_MAP_GRAPH_EDGE_COUNT (CONFIG_MAP_VALIDATION_NODE_COUNT - 1)

typedef struct ConfigMapGraphNode ConfigMapGraphNode;

typedef struct {
    ConfigMapGraphNode *from;
    ConfigMapGraphNode *to;
} ConfigMapGraphEdge;

struct ConfigMapGraphNode {
    // 이 맵의 값 타입을 키 타입으로 쓰는 맵들로 향하는 간선
    ConfigMapGraphEdge *edges[CONFIG_MAP_GRAPH_EDGE_COUNT];
    ConfigMapValidationSubject *subject;
    size_t edge_count;
    size_t degree;
};

typedef struct {
    ConfigMapGraphNode nodes[CONFIG_MAP_VALIDATION_NODE_COUNT];
    size_t node_count;
} ConfigMapGraph;

typedef struct {
    Value_t value;                       // 맵에 존재하지 않는 키
    ConfigMapValidationSubject *subject; // value를 키로 갖지 않는 ConfigMap
} ConfigValidatorLog;

// 검증 목록
static ConfigMapValidationSubject config_map_validation_subjects[CONFIG_MAP_VALIDATION_NODE_COUNT];
static size_t config_map_validation_subject_count = 0;

// 간선 메모리 풀
static ConfigMapGraphEdge config_graph_map_graph_edge_pool[CONFIG_MAP_GRAPH_EDGE_COUNT];
static size_t config_graph_map_graph_edge_pool_count = 0;

// 그래프
static ConfigMapGraph graph;

// 로그
#define LOG_COUNT 128
static ConfigValidatorLog logs[LOG_COUNT];
static size_t log_count = 0;

// 검증에 사용할 큐 자료구조
#define QUEUE_CAPACITY (CONFIG_MAP_VALIDATION_NODE_COUNT + 1)
static ConfigMapGraphNode *queue[QUEUE_CAPACITY];
static size_t queue_front, queue_rear;

static void InitQueue();
static void EnqueueConfigMapGraphNode(ConfigMapGraphNode *node);
static ConfigMapGraphNode *DequeuConfigMapGraphNode();
static bool IsQueueEmpty();
static bool IsQueueFull();

static ConfigMapGraphEdge *CreateConfigMapGraphEdge(ConfigMapGraphNode *node_a,
                                                    ConfigMapGraphNode *node_b);
static void AddConfigMapGraphEdge(ConfigMapGraphNode *from, ConfigMapGraphNode *to);
static void InitConfigMapGraph();
static void TopologySort();
static ConfigValidatorResult Validate();
static ConfigValidatorResult ValidateEdgeKeyCoverage(ConfigMapGraphNode *node);
static void AddConfigValidatorLog(ConfigMapValidationSubject *subject, Value_t value);

Hash_t ConfigValidator_TypeToHash(const char *type_name)
{
    return djb2(type_name);
}

static void InitQueue()
{
    queue_front = 0;
    queue_rear = 0;
}

static void EnqueueConfigMapGraphNode(ConfigMapGraphNode *node)
{
    if (IsQueueFull()) {
        return;
    }
    queue[queue_front] = node;
    queue_front = (queue_front + 1) % QUEUE_CAPACITY;
}

static ConfigMapGraphNode *DequeuConfigMapGraphNode()
{
    size_t index = queue_rear;
    if (IsQueueEmpty()) {
        return NULL;
    }
    queue_rear = (queue_rear + 1) % QUEUE_CAPACITY;
    return queue[index];
}

static bool IsQueueEmpty()
{
    return queue_front == queue_rear;
}

static bool IsQueueFull()
{
    return (queue_front + 1) % QUEUE_CAPACITY == queue_rear;
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
    // 1. 그래프 생성
    // 2. 생성된 그래프의 위상을 따져 가장 낮은 위상부터 탐색
    // 3. 한 맵(A)의 값 타입을 키로 사용하는 다른 맵(B)과 관계를 본다.
    //    A의 모든 값이 B의 모든 엔트리에 키로 존재하는지 확인한다.

    InitConfigMapGraph();
    TopologySort();
    return Validate();
}

static void InitConfigMapGraph()
{
    graph.node_count = 0;
    log_count = 0;

    // Node 생성
    // 노드는 위상 정렬에 필요하며, 탐색을 시작할 노드들을 선택하기위해 필요하다.
    // 이 때 각 Node는 validation_subject와 1대1로 대응된다. 예를 들어 3번째 Node는 3번쨰
    // validation_subject다.
    for (size_t i = 0; i < config_map_validation_subject_count; i++) {
        graph.nodes[graph.node_count++] = (ConfigMapGraphNode){
            .degree = 0, .edge_count = 0, .subject = &config_map_validation_subjects[i]};
    }
}

ConfigMapGraphEdge *CreateConfigMapGraphEdge(ConfigMapGraphNode *node_a, ConfigMapGraphNode *node_b)
{
    config_graph_map_graph_edge_pool[config_graph_map_graph_edge_pool_count].from = node_a;
    config_graph_map_graph_edge_pool[config_graph_map_graph_edge_pool_count].to = node_b;
    return &config_graph_map_graph_edge_pool[config_graph_map_graph_edge_pool_count++];
}

void AddConfigMapGraphEdge(ConfigMapGraphNode *from, ConfigMapGraphNode *to)
{
    from->edges[from->edge_count++] = CreateConfigMapGraphEdge(from, to);
}

static void TopologySort()
{
    // 간선은 한 맵의 값 타입이 다른 맵의 키 타입으로 쓰일 때 생기므로, O(n^2)의 시간복잡도로
    // 수행된다.
    // 간선이 발견될 때 차수를 누적한다.

    for (size_t i = 0; i < config_map_validation_subject_count; i++) {
        for (size_t j = 0; j < config_map_validation_subject_count; j++) {
            if (i == j) {
                continue;
            }
            Hash_t map_a_value_hash = config_map_validation_subjects[i].value_hash;
            Hash_t map_b_key_hash = config_map_validation_subjects[j].key_hash;
            if (map_a_value_hash == map_b_key_hash) {
                graph.nodes[j].degree++;
                AddConfigMapGraphEdge(&graph.nodes[i], &graph.nodes[j]);
            }
        }
    }
}

static ConfigValidatorResult Validate()
{
    ConfigValidatorResult result = CONFIG_VALIDATOR_RESULT_OK;
    ConfigMapGraphNode *node;

    InitQueue();
    for (size_t i = 0; i < graph.node_count; i++) {
        if (graph.nodes[i].degree == 0 && graph.nodes[i].edge_count > 0) {
            EnqueueConfigMapGraphNode(&graph.nodes[i]);
        }
    }
    while (!IsQueueEmpty()) {
        node = DequeuConfigMapGraphNode();

        if (ValidateEdgeKeyCoverage(node) == CONFIG_VALIDATOR_RESULT_ERROR) {
            result = CONFIG_VALIDATOR_RESULT_ERROR;
        }
    }
    return result;
}

ConfigValidatorResult ValidateEdgeKeyCoverage(ConfigMapGraphNode *node)
{
    // 각 edge마다, from 노드의 모든 값이 to 노드의 모든 키로 존재하는지 확인하기
    ConfigMapGraphEdge *edge;
    ConfigMapGraphNode *from, *to;
    Value_t value;
    ConfigValidatorResult result = CONFIG_VALIDATOR_RESULT_OK;
    for (size_t i = 0; i < node->edge_count; i++) {
        edge = node->edges[i];
        from = edge->from;
        to = edge->to;
        for (ConfigMapCount_t j = 0; j < from->subject->map->count; j++) {
            value = from->subject->map->entries[j].value;
            if (ConfigMap_Contains(to->subject->map, (Key_t)value) != CONFIG_MAP_RESULT_OK) {
                AddConfigValidatorLog(to->subject, value);
                result = CONFIG_VALIDATOR_RESULT_ERROR;
            }
        }
        to->degree--;
        if (to->degree == 0) {
            EnqueueConfigMapGraphNode(to);
        }
    }
    return result;
}

static void AddConfigValidatorLog(ConfigMapValidationSubject *subject, Value_t value)
{
    logs[log_count++] = (ConfigValidatorLog){.subject = subject, .value = value};
}
