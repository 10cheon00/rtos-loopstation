#ifndef CONFIG_MAP_H
#define CONFIG_MAP_H

/**
 * 설정에서 사용되는 자료구조인 맵과 테이블에 필수로 작성되어야할 설정값이 없는 경우를 검증한다.
 * 맵과 테이블을 접근하기 위해 키를 사용하는데, 자료구조 내에 이 키에 대응되는 설정값이 없는 경우를
 * 검증한다. 키는 enum으로 정의된 ID들을 의미한다.
 *
 * 맵 = 키와 키에 대응되는 어떤 값을 하나의 엔트리로 정의할 때 그 엔트리들의 배열
 * 테이블 = 키에 대응되는 어떤 값을 타입으로 하는 배열을 정의하되 키의 개수를 길이로 갖는 배열
 *
 * 자료구조 안에 키에 대응되는 값이 있냐 없냐를 검증하기에 앞서서, 어떤 키가 어떤 자료구조와 관련이
 * 있는지 알아야 한다. 그 관계를 정의하려면 설정 자료구조를 생성할 때 이에 대한 정보를 제공해야한다.
 * 키와 데이터 간 관계를 가진 그래프를 정의할 수만 있다면, 런타임에 해당 키에 관련된 모든 자료구조를
 * 조회할 수 있다.
 *
 * 설정 자료구조를 정의할 때 검증자가 제공하는 매크로를 사용하여 검증 함수를 호출할 때 해당
 * 자료구조를 조회할 수 있다. 따라서, 설정 파일을 작성할 때 이 헤더 파일에 있는 매크로를 사용하여야
 * 한다.
 *
 * 여기에 정의된 함수와 매크로는 __attribute__((constructor))가 붙어 있으므로 main 함수가 호출되기
 * 직전에 호출된다. 함수가 호출될 때 설정 자료구조들이 등록되며, 검증 함수가 호출될 때 등록된
 * 자료구조들을 훑게 된다.
 *
 * 검증 절차는 다음과 같다.
 * 1. 등록된 키와 데이터 관계를 그래프에 나타낸다.
 * 2. 그래프를 깊이 우선 탐색하여 해당 키와 관계된 모든 설정들이 정상적으로 작성되어 있는지
 * 검증한다.
 * 3. 검증 과정에서 오류가 발견되면 로그를 남긴다.
 * 4. 검증 결과를 호출자에게 반환한다.
 *
 * 맵 자료구조는 값이 있는지 없는지 알 수 있지만, 테이블 자료구조는 값이 항상 있으므로 유무를 검사할
 * 수 없다. 그러므로 맵 자료구조만 사용해서 설정파일을 작성해야한다.
 *
 * 이 파일은 맵 자료구조 템플릿을 제공한다.
 */

#include <stddef.h>

#include "utils.h"

typedef uint32_t Key_t;
typedef uint32_t Value_t;
typedef uint8_t ConfigMapCount_t;

typedef enum {
    CONFIG_MAP_RESULT_ERROR = 0,
    CONFIG_MAP_RESULT_OK,
} ConfigMapResult;

typedef struct {
    Key_t key;
    Value_t value;
} ConfigMapEntry;

typedef struct {
    ConfigMapEntry *entries;
    ConfigMapCount_t count;
} ConfigMap;

ConfigMapResult ConfigMap_Get(ConfigMap *map, Key_t key, Value_t *value);

#endif
