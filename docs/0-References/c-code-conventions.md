---
title: C 코드 컨벤션
version: 0.1.0
change_history:
  - date: 2026-07-30
    version: 0.1.0
    summary: App 코드의 포맷, 명명, 타입, 함수, RTOS 시간값과 오류 처리 규칙을 정리함
---

# C 코드 컨벤션

## 1. 문서 범위

- 이 문서는 프로젝트가 직접 작성하는 `App/` 아래의 C 소스와 헤더에 적용한다.
- `App/libraries/`의 외부 라이브러리는 해당 라이브러리의 원래 코드 스타일을 우선한다.
- STM32CubeMX가 관리하는 `Core/`와 자동 생성 코드는 생성 도구의 형식과 USER CODE 영역 규칙을 우선한다.
- 폴더 배치와 파일명은 [폴더 및 파일명 컨벤션](./folder-file-conventions.md)을 따른다.
- 기존 코드가 이 문서와 다른 경우 기존 표현을 새 규칙의 근거로 삼지 않고 수정 대상 또는 호환이 필요한 예외로 본다.

## 2. 자동 포맷

- 프로젝트 루트의 `.clang-format`을 C 코드 포맷의 단일 기준으로 사용한다.
- 변경한 `.c`와 `.h` 파일은 커밋 전에 `clang-format -i <변경한 파일>`로 정렬한다.
- 들여쓰기는 공백 4칸을 사용하고 탭을 사용하지 않는다.
- 한 줄은 최대 100자를 기준으로 줄바꿈한다.
- 함수 정의의 여는 중괄호는 다음 줄에 둔다.
- `if`, `for`, `while`, `switch` 같은 제어문의 여는 중괄호는 같은 줄에 둔다.
- 한 줄 조건문과 한 줄 반복문을 작성하지 않는다.
- 빈 함수만 한 줄 함수 표현을 허용한다.
- 포인터 별표는 타입이 아니라 변수 이름 쪽에 붙인다.
- 파일 마지막에는 개행을 둔다.
- 수동 정렬을 위해 연속된 대입문이나 선언문에 임의의 공백 열을 만들지 않는다.

```c
static TaskStatus HandleEvent(const StateEvent *event)
{
    if (event == NULL) {
        return TASK_STATUS_ERROR;
    }

    return TASK_STATUS_OK;
}
```

## 3. include

- `.c` 파일은 대응하는 자신의 헤더를 가장 먼저 include한다.
- 자신의 헤더 다음에는 표준 라이브러리, 외부 프레임워크, 프로젝트 헤더 순으로 그룹을 나눈다.
- 의미가 다른 include 그룹 사이는 빈 줄 하나로 구분한다.
- `.clang-format`의 `SortIncludes: false`와 `IncludeBlocks: Preserve` 설정에 따라 include 순서를 자동 변경하지 않는다.
- 각 파일은 자신이 직접 사용하는 타입과 함수의 헤더를 include하고 전이 include에 의존하지 않는다.
- 프로젝트 include 경로가 등록되어 있으면 `../../../` 형태의 상대 경로를 사용하지 않는다.
- 헤더 가드는 파일 기본 이름을 대문자 `SNAKE_CASE`로 바꾼 이름을 사용한다.

```c
#include "input_task.h"

#include <stdint.h>

#include "cmsis_os2.h"

#include "input_messages.h"
#include "mcp23017.h"
```

## 4. 공통 명명

- 프로젝트가 정의하는 타입 이름은 `PascalCase`를 사용한다.
- 함수 이름은 단어의 시작을 대문자로 쓰는 `PascalCase` 계열을 사용한다.
- 변수, 함수 인자, 구조체 멤버와 배열 이름은 소문자 `snake_case`를 사용한다.
- 매크로와 enum 상수는 대문자 `SNAKE_CASE`를 사용한다.
- 이름에는 구현 수단보다 도메인 의미와 책임을 우선해서 표현한다.
- 약어를 새로 만들기보다 `Ui`, `Id`, `I2c`, `Lcd`처럼 프로젝트에서 정한 단어 표기를 유지한다.
- 동일 개념에 `UI`와 `Ui`, `ID`와 `Id` 같은 표기를 혼용하지 않는다.
- 단위가 있는 값은 이름에 단위를 붙인다.
- RTOS tick 값은 `_ticks`, 밀리초 값은 `_ms` 접미사를 사용한다.

```c
typedef struct {
    uint32_t timestamp_ticks;
    ButtonId button_id;
} ButtonPayload;
```

## 5. 공개 함수

- 헤더에 공개하는 함수는 `<Module>_<Action>` 형식으로 모듈 접두사를 사용한다.
- 모듈 접두사는 해당 API의 소유 타입이나 파일 책임과 일치시킨다.
- 함수의 동작은 `Init`, `Run`, `Get`, `Find`, `Read`, `Write`, `Copy`, `Process`처럼 구체적인 동사로 표현한다.
- 조회 함수는 값을 반환하는지, 출력 인자에 기록하는지 시그니처와 이름에서 일관되게 표현한다.
- 작업이 적용되지 않는 것이 정상 흐름이라면 `Try` 접두사를 사용하고, 미적용과 오류를 반환값에서 구분한다.
- 함수가 입력 객체를 변경하지 않으면 포인터 인자에 `const`를 사용한다.
- 출력 인자는 입력 인자 뒤에 배치한다.
- 포인터 인자를 받는 공개 함수는 `NULL` 허용 여부를 계약이나 검증 코드로 명확히 한다.

```c
Mcp23017Status Mcp23017_ReadRegister(
    I2C_HandleTypeDef *hi2c,
    uint8_t address,
    uint8_t reg,
    uint8_t *value);
```

## 6. 파일 내부 함수와 심볼

- 현재 `.c` 파일에서만 사용하는 함수와 변수에는 `static`을 붙인다.
- 파일 내부 함수는 이미 파일이 소유 문맥을 제공하므로 태스크나 모듈 접두사를 반복하지 않는다.
- 태스크 내부 함수는 `HandleInputEvent`, `TryUpdateParameter`, `IsValidInitParams`처럼 역할부터 이름에 적는다.
- 파일 내부 함수의 전방 선언은 include와 파일 범위 타입 및 변수 선언 다음에 둔다.
- 다른 파일이 직접 사용할 필요가 없는 배열, 설정 테이블과 개수 심볼도 `static const`로 제한한다.
- 외부에 노출해야 하는 설정은 헤더에 `extern const`로 선언한다.
- 쓰기 가능한 전역 변수는 만들지 않는 것을 기본으로 하고, 필요한 런타임 상태는 소유 모듈의 파일 범위 `static` 변수나 명시적 context 구조체에 둔다.

```c
static osMessageQueueId_t state_event_queue;

static TaskStatus HandleStateEvent(const StateEvent *event);
static int IsValidInitParams(const StateInitParams *params);
```

## 7. 타입

- 식별자 enum 타입은 `<Subject>Id`로 이름을 구성한다.
- 상태값 enum 타입은 `<Subject>State`, 처리 상태는 `<Subject>Status`로 이름을 구성한다.
- 특정 호출의 처리 결과는 `<Operation>Result`로 이름을 구성한다.
- 태스크나 모듈의 초기 의존성 묶음은 `<Module>InitParams`로 이름을 구성한다.
- 런타임에 함께 유지되는 값은 `<Module>Context`로 묶되 초기화 인자와 혼용하지 않는다.
- 태스크 간 메시지는 payload 구조체와 type enum을 union envelope 구조체로 묶는다.
- map의 한 행을 나타내는 구조체는 `<Relation>MapEntry`로 이름을 구성한다.
- 단순 인덱스 조회 테이블은 `<Subject>Table`, 객체 등록 및 조회 집합은 `<Subject>Registry` 사용을 검토한다.
- 외부 라이브러리와 STM32 HAL이 정의한 `_t`, `HandleTypeDef` 같은 타입 이름은 변경하지 않는다.

```c
typedef struct {
    ButtonId button_id;
    UiActionId ui_action_id;
} ButtonUiActionMapEntry;
```

## 8. enum과 결과값

- 성공 여부를 나타내는 enum은 `*_OK = 0`을 첫 값으로 명시한다.
- 오류, 무시, 미적용과 실제 변경 성공을 하나의 값으로 뭉치지 않는다.
- enum의 마지막 항목 뒤에도 쉼표를 둔다.
- 유효하지 않거나 선택되지 않은 식별자가 필요하면 `*_NONE`을 정의한다.
- 배열 인덱스 상한이 필요하면 마지막에 `*_COUNT`를 두고 실제 도메인 값으로 사용하지 않는다.
- 비트 플래그는 각 값의 비트 위치를 명시하고 일반 순차 enum과 구분한다.
- 함수의 성공 여부와 도메인 처리 결과가 다르면 `TaskStatus` 하나로 합치지 않고 별도 result 타입을 사용한다.

```c
typedef enum {
    PARAMETER_UPDATE_RESULT_OK = 0,
    PARAMETER_UPDATE_RESULT_NOT_APPLICABLE,
    PARAMETER_UPDATE_RESULT_UNCHANGED,
    PARAMETER_UPDATE_RESULT_ERROR,
} ParameterUpdateResult;
```

## 9. 변수와 상수

- 변수는 가장 좁은 유효 범위에 선언한다.
- 포인터 변수에 `p_`, 전역 변수에 `g_` 같은 헝가리안 접두사를 붙이지 않는다.
- 포인터의 부재는 `NULL`, 정수의 0은 `0`으로 표현한다.
- 변경하지 않는 입력 포인터와 설정 배열에는 `const`를 사용한다.
- 배열 길이는 `size_t`를 기본으로 사용한다.
- 배열 길이는 `ARRAY_COUNT(array)`로 계산하고 같은 배열과 함께 정의한다.
- 배열 개수 심볼은 `<array_name>_count`로 이름을 구성한다.
- 여러 멤버를 초기화할 때는 지정 초기화 문법을 사용한다.
- 서로 다른 의미의 변수를 한 선언문에 묶지 않는다.
- 부호 있는 값과 부호 없는 값을 혼합해 계산할 때 변환과 범위를 명시적으로 검토한다.

```c
static const ButtonUiActionMapEntry button_ui_action_map[] = {
    {
        .button_id = BUTTON_ID_LEFT,
        .ui_action_id = UI_ACTION_NAVIGATE_LEFT,
    },
};

static const size_t button_ui_action_map_count =
    ARRAY_COUNT(button_ui_action_map);
```

## 10. 함수 구성과 오류 처리

- 함수는 하나의 책임을 가지며 이름에 없는 후속 작업을 숨겨 수행하지 않는다.
- 이벤트 변환, 상태 전이, 파라미터 변경과 queue 전송은 필요에 따라 결과를 반환한 뒤 상위 조정 함수에서 순서를 결정한다.
- 예상 가능한 미적용은 오류와 구분하고, 오류가 발생하면 가능한 한 즉시 반환한다.
- HAL, CMSIS-RTOS와 드라이버 반환값은 호출 직후 검사한다.
- 반환값을 의도적으로 무시하는 경우에는 이유를 주석이나 명시적 캐스팅으로 남긴다.
- 함수가 여러 결과를 반환해야 하면 결과 구조체를 사용하고 의미가 다른 출력 인자를 계속 추가하지 않는다.
- 초기화 인자는 태스크 시작 시 검증하고, 잘못된 포인터를 파일 범위 상태에 저장하지 않는다.
- 상태 변경과 렌더링처럼 한 이벤트에서 중복될 수 있는 부수 효과는 결과를 취합한 뒤 한 곳에서 수행한다.

## 11. RTOS와 시간값

- ISR과 태스크 사이에 전달하는 시간은 FreeRTOS tick 기준으로 기록하고 `_ticks` 접미사를 사용한다.
- 실제 밀리초 상수는 `_MS`, 변환된 tick 상수는 `_TICKS` 접미사를 사용한다.
- 밀리초를 RTOS API에 직접 전달하지 않고 `pdMS_TO_TICKS()`로 변환한다.
- queue handle과 peripheral handle은 이를 소유하거나 주입받은 태스크가 보관한다.
- 태스크 전용 queue와 handle을 다른 태스크가 전역 심볼로 직접 참조하지 않는다.
- ISR에서는 블로킹 작업과 장치 통신을 수행하지 않고 필요한 최소 이벤트만 기록한다.
- queue 전송과 수신 결과는 무시하지 않고 실패 정책을 호출 위치에서 결정한다.

```c
#define INPUT_TASK_TIMEOUT_MS 500UL
#define INPUT_TASK_TIMEOUT_TICKS \
    (pdMS_TO_TICKS(INPUT_TASK_TIMEOUT_MS))
```

## 12. 헤더

- 헤더에는 해당 모듈의 공개 타입, 상수와 함수 선언만 둔다.
- 파일 내부 helper, 내부 배열과 변경 가능한 상태는 헤더에 노출하지 않는다.
- 선언에 필요한 헤더만 include하고 구현에만 필요한 헤더는 `.c` 파일에 둔다.
- 순환 참조를 피할 수 있으면 구조체 전방 선언을 사용한다.
- 구조체 크기나 멤버 접근이 공개 계약이 아니면 opaque type 사용을 검토한다.
- 다른 파일이 정적 구조체 객체를 값으로 생성해야 한다면 opaque type을 사용하지 않고 초기화 규칙을 별도 API나 매크로로 명시한다.
- 헤더 자체만 include한 번역 단위에서도 필요한 타입이 모두 선언되어야 한다.

## 13. 주석과 미완료 항목

- 주석은 코드가 무엇을 하는지 반복하지 않고 제약, 이유, 하드웨어 특성 또는 처리 순서를 설명한다.
- 복잡한 처리 단계는 짧은 블록 주석이나 번호 목록으로 설명한다.
- 공개 API에 호출 조건이나 부수 효과가 있으면 선언 가까이에 문서 주석을 둔다.
- 미완료 작업은 `TODO` 키워드 뒤에 콜론을 붙여 표시하고 바로 아래에 구체적인 남은 작업을 적는다.
- 해결 방향이 정해지지 않은 긴 설계 토론은 코드 주석에 누적하지 않고 설계 또는 기능 문서로 이동한다.
- 주석은 현재 구현과 함께 갱신하고 더 이상 유효하지 않은 과거 설명은 제거한다.

## 14. 변경 확인

- 공개 타입, enum 값 또는 메시지 구조를 변경하면 모든 생산자와 소비자를 함께 검색한다.
- 파일을 추가하거나 삭제하면 `App/CMakeLists.txt`의 source 및 include 경로를 확인한다.
- 이름을 변경하면 선언, 정의, include, CMake 항목과 문서의 코드 식별자를 함께 갱신한다.
- 포맷 적용 후 프로젝트를 빌드하고 새 컴파일 경고가 없는지 확인한다.
- 코드 컨벤션 변경 자체는 별도 `refactor` 커밋으로 분리하고 기능 변경과 섞지 않는다.
