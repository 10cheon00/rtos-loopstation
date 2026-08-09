#include "config_validator.h"

#include "config_map.h"

#define CONFIG_MAP_VALIDATION_NODE_COUNT 16

typedef struct {
    // 이 맵의 값 타입을 키 타입으로 쓰는 맵들로 향하는 간선
    // ConfigMapGraphEdge edges[CONFIG_MAP_MEMORY_POOL_MAX_COUNT - 1];
    size_t edge_count;
    size_t degree;
} ConfigMapGraphNode;

// typedef struct {
//     ConfigMapGraphNode *from;
//     ConfigMapGraphNode *to;
// } ConfigMapGraphEdge;

typedef struct {
    ConfigMapGraphNode nodes[CONFIG_MAP_VALIDATION_NODE_COUNT];
    // ConfigMapGraphEdge
    //     edges[CONFIG_MAP_MEMORY_POOL_MAX_COUNT - 1]; // 비 사이클 그래프에서 한 노드로 향하는
    //     간선의 수가 1개이므로 간선의 수는 노드 수 - 1임
    size_t node_count;
    size_t edge_count;
} ConfigMapGraph;

static ConfigMapValidationSubject config_map_validation_subjects[CONFIG_MAP_VALIDATION_NODE_COUNT];
static size_t config_map_validation_subject_count = 0;

static void GenerateConfigMapGraph(ConfigMapGraph *graph);
static void TopologySort(ConfigMapGraph *graph);
static ConfigValidatorResult Dfs(ConfigMapGraphNode *node);

Hash ConfigValidator_TypeToHash(const char *type_name)
{
    return djb2(type_name);
}

ConfigValidatorResult ConfigValidator_AddConfigMap(ConfigMap *map, Hash key_hash, Hash value_hash)
{
    if (config_map_validation_subject_count == CONFIG_MAP_VALIDATION_NODE_COUNT) {
        return CONFIG_MAP_RESULT_ERROR;
    }
    config_map_validation_subjects[config_map_validation_subject_count++] =
        (ConfigMapValidationSubject){.map = map, .key_hash = key_hash, .value_hash = value_hash};
    return CONFIG_MAP_RESULT_OK;
}

ConfigValidatorResult ConfigValidator_Validate()
{
    // 1. 그래프 생성
    // 2. 생성된 그래프의 위상을 따져 가장 낮은 위상부터 탐색
    // 3. 한 맵(A)의 값 타입을 키로 사용하는 다른 맵(B)과 관계를 본다.
    //    A의 모든 값이 B의 모든 엔트리에 키로 존재하는지 확인한다.
    ConfigMapGraph graph;

    GenerateConfigMapGraph(&graph);
    TopologySort(&graph);

    for (size_t i = 0; i < graph.node_count; i++) {
        if (graph.nodes[i].degree > 0) {
            continue;
        }
        if (Dfs(&graph.nodes[i]) == CONFIG_VALIDATOR_RESULT_ERROR) {
            // TODO:
            // 설정 오류 로그 남기기
        }
    }

    return CONFIG_VALIDATOR_RESULT_OK;
}

static void GenerateConfigMapGraph(ConfigMapGraph *graph)
{
    graph->edge_count = 0;
    graph->node_count = 0;

    // Node 생성
    // 노드는 위상 정렬에 필요하며, 탐색을 시작할 노드들을 선택하기위해 필요하다.
    // 이 때 각 Node는 validation_subject와 1대1로 대응된다. 예를 들어 3번째 Node는 3번쨰
    // validation_subject다.
    for (size_t i = 0; i < config_map_validation_subject_count; i++) {
        graph->nodes[graph->node_count++] = (ConfigMapGraphNode){.degree = 0, .edge_count = 0};
    }
}

static void TopologySort(ConfigMapGraph *graph)
{
    // 간선은 한 맵의 값 타입이 다른 맵의 키 타입으로 쓰일 때 생기므로, O(n^2)의 시간복잡도로
    // 수행된다.
    // 간선이 발견될 때 차수를 누적한다.

    for (size_t i = 0; i < config_map_validation_subject_count; i++) {
        for (size_t j = i + 1; j < config_map_validation_subject_count; j++) {
            Hash map_a_value_hash = config_map_validation_subjects[i].value_hash;
            Hash map_b_key_hash = config_map_validation_subjects[j].key_hash;
            if (map_a_value_hash == map_b_key_hash) {
                graph->nodes[j].degree++;
                // ConfigMap *map_a = config_map_validation_subjects[i].map;
                // ConfigMap *map_b = config_map_validation_subjects[j].map;
                // graph->edges[graph->edge_count++] =
                // (ConfigMapGraphEdge){.from = map_a, .to = map_b};
            }
        }
    }
}

ConfigValidatorResult Dfs(ConfigMapGraphNode *node)
{
    return CONFIG_VALIDATOR_RESULT_OK;
}
