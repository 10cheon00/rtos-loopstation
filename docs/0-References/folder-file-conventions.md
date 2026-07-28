---
title: 폴더 및 파일명 컨벤션
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: App 코드의 폴더 구조와 파일명 및 소유권 기반 배치 규칙을 작성함
---

# 폴더 및 파일명 컨벤션

## 1. 문서 범위

- 이 문서는 `App/` 아래에 프로젝트 코드를 추가하거나 이동할 때 적용할 폴더 구조와 파일명 규칙을 정의한다.
- `docs/0-References/`는 폴더 컨벤션, 코드 컨벤션과 프로젝트 전반에서 반복해서 참조하는 문서를 보관한다.
- STM32CubeMX가 관리하는 `Core/`와 `cmake/stm32cubemx/`의 생성 파일에는 이 문서의 구조를 강제하지 않는다.
- `App/libraries/`에 반입한 외부 라이브러리는 해당 라이브러리의 원래 구조와 파일명을 우선한다.

## 2. 공통 명명 규칙

- 프로젝트가 직접 작성하는 폴더와 파일 이름은 영문 소문자 `snake_case`를 사용한다.
- C 소스와 헤더는 각각 `.c`와 `.h` 확장자를 사용한다.
- 같은 모듈의 공개 선언과 구현은 가능한 한 같은 기본 이름의 `.h`와 `.c` 쌍으로 작성한다.
- 파일 이름은 `common`, `manager`, `helper`처럼 책임이 넓거나 모호한 단어만으로 짓지 않는다.
- 파일 이름에는 해당 파일이 소유하는 대상이나 정책이 드러나야 한다.
- 여러 항목을 모은 디렉터리는 `drivers`, `messages`, `tasks`, `types`, `configs`, `stores`, `ui_panels`처럼 복수형을 사용한다.
- 하나의 개념이나 실행 단위를 나타내는 디렉터리는 `input`, `display`, `state`, `state_machine`처럼 단수형 또는 고유 도메인명을 사용한다.
- 약어는 프로젝트에서 이미 정한 표기를 유지하되 파일명에서는 소문자로 작성한다.
- 파일명 변경 시 include 문, `App/CMakeLists.txt`의 source 및 include 경로도 함께 갱신한다.

## 3. App 최상위 구조

- `App/app.h`는 여러 App 모듈에서 공통으로 필요한 최소 애플리케이션 선언만 제공한다.
- `App/CMakeLists.txt`는 App 소스, 공개 include 경로와 외부 target 의존성을 등록한다.
- `App/drivers/`는 특정 하드웨어 장치와 직접 통신하는 드라이버를 보관한다.
- `App/libraries/`는 u8g2처럼 프로젝트 외부에서 가져온 라이브러리를 보관한다.
- `App/messages/`는 태스크 또는 모듈 경계를 넘어 전달되는 메시지 타입을 보관한다.
- `App/tasks/`는 RTOS 태스크와 그 태스크가 소유하는 기능 코드를 보관한다.
- `App/types/`는 특정 태스크가 단독 소유하지 않고 여러 App 모듈이 공유하는 도메인 타입을 보관한다.
- `App/utils/`는 특정 도메인이나 하드웨어에 의존하지 않는 작은 범용 기능을 보관한다.
- 특정 태스크가 소유하는 타입, 설정 또는 보조 함수는 편의를 이유로 App 최상위 공통 폴더에 올리지 않는다.

## 4. 태스크 폴더

- 각 태스크는 `App/tasks/<task_name>/` 폴더 하나를 소유한다.
- `<task_name>`은 `input`, `display`, `state`처럼 태스크의 도메인을 나타내는 소문자 이름을 사용한다.
- 태스크 진입점은 `<task_name>_task.h`와 `<task_name>_task.c`로 작성한다.
- 태스크 초기화 인자 계약은 `<task_name>_initparams.h`로 작성한다.
- `*_initparams.h`는 설정 저장소가 아니라 composition root가 태스크에 자원을 주입하기 위한 계약으로 취급한다.
- 태스크 전용 코드는 다른 태스크 폴더에서 직접 include하지 않는다.
- 다른 태스크가 필요한 정보는 `App/messages/`의 메시지 또는 `App/types/`의 공유 타입을 통해 전달한다.
- 태스크 하위 폴더는 실제로 둘 이상의 관련 파일을 묶을 필요가 있을 때만 추가한다.
- 태스크 하위 폴더의 이름은 기술 계층보다 소유 역할을 나타내도록 정한다.

## 5. 태스크 전용 configs

- 한 태스크가 해석하고 소유하는 정적 설정은 `App/tasks/<task_name>/configs/`에 둔다.
- 같은 태스크의 여러 `.c` 파일이 설정을 참조하더라도 설정의 의미를 해당 태스크가 결정한다면 태스크 전용 설정으로 유지한다.
- 설정 파일 이름에는 설정 대상을 포함한다.
- ID와 대상의 관계를 정의하는 파일은 `<subject>_mapping.h`와 `<subject>_mapping.c`로 작성한다.
- 한 대상과 여러 값의 관계를 정의하는 파일은 `<subject>_binding.h`와 `<subject>_binding.c`로 작성한다.
- 장치 인스턴스 목록을 정의하는 파일은 `<subject>_devices.h`와 `<subject>_devices.c`로 작성한다.
- 설정 구조체 타입과 `extern const` 선언은 헤더에 둔다.
- 설정 배열과 배열 길이 정의는 같은 `.c` 파일에 둔다.
- 설정 배열의 이름은 대상과 복수 항목임을 드러내는 복수형을 사용한다.
- 설정 배열 길이 이름은 배열 이름과 연결되는 `_count` 접미사를 사용한다.
- 설정을 소비하는 모듈은 설정 배열을 수정하지 않으며 선언에는 `const`를 사용한다.
- 여러 태스크가 동일 설정을 직접 해석해야 하는 경우에만 `App/configs/` 같은 애플리케이션 공통 설정 폴더 신설을 검토한다.
- 공통 설정 폴더를 신설하기 전에 메시지 전달이나 소유 모듈의 조회 API로 공유할 수 있는지 먼저 확인한다.

## 6. 상태 관리 하위 구조

- 상태 머신 공통 구현은 `App/tasks/state/state_machine/`에 둔다.
- 모든 상태 머신이 공유하는 `State`, `StateMachine`, `StateTransition` 계약은 `state.h/.c` 또는 `state_machine.h/.c`에 둔다.
- 개별 상태 구현은 `App/tasks/state/state_machine/states/<state_category>/`에 둔다.
- 상태 분류 이름은 `ui`, `system`, `track`처럼 요구사항과 아키텍처의 상태 머신 이름을 사용한다.
- 개별 상태 파일은 `<state_category>_state_<state_name>.h/.c` 순서로 이름을 구성한다.
- UI 상태 파일은 현재 사용 중인 `ui_state_<panel_name>_panel.h/.c` 형식을 유지한다.
- 상태 ID와 정적 `State` 인스턴스의 관계는 StateTask가 소유하는 `configs/`에 mapping 파일로 둔다.
- 상태 관리 태스크가 해석하는 패널-파라미터 관계는 `App/tasks/state/configs/`에 binding 파일로 둔다.
- 상태 머신 상태 파일은 파라미터 저장소나 표시 queue를 직접 참조하지 않는다.
- 런타임 상태 저장소는 `App/tasks/state/stores/`에 둔다.
- 저장소 파일은 `<subject>_store.h/.c`로 이름을 구성한다.

## 7. 디스플레이 하위 구조

- DisplayTask 진입점과 초기화 계약은 `App/tasks/display/` 바로 아래에 둔다.
- LCD 패널별 drawing 코드는 `App/tasks/display/ui_panels/`에 둔다.
- 패널 renderer 파일은 `ui_<panel_name>_panel.h/.c`로 작성한다.
- 여러 패널이 공유하는 drawing primitive는 `ui.h/.c`에 둔다.
- `UiPanelId`와 renderer 함수의 관계는 `ui_panel_renderer.h/.c`에 둔다.
- renderer mapping을 다른 표시 설정과 분리할 필요가 생기면 `App/tasks/display/ui_panels/configs/`로 이동한다.
- DisplayTask만 사용하는 renderer와 drawing 함수는 `App/types/`나 `App/utils/`로 이동하지 않는다.

## 8. 드라이버와 라이브러리

- 하드웨어 장치 드라이버는 장치 또는 모듈 이름을 사용한 `<device_name>.h/.c` 쌍으로 작성한다.
- 드라이버는 `ControlButtonId`, `UiPanelId`처럼 상위 애플리케이션 의미를 나타내는 타입을 알지 않는다.
- 드라이버 공개 헤더에는 호출자가 장치와 통신하는 데 필요한 타입과 API만 둔다.
- 특정 태스크의 장치 목록과 핀-의미 매핑은 드라이버가 아니라 해당 태스크의 `configs/`에 둔다.
- 외부 라이브러리 코드는 `App/libraries/<library_name>/` 아래에 둔다.
- 외부 라이브러리 내부 파일명과 폴더 구조는 프로젝트 컨벤션에 맞추기 위해 임의로 변경하지 않는다.
- 외부 라이브러리를 감싸는 프로젝트 전용 코드는 소유 태스크 또는 드라이버 폴더에 둔다.

## 9. 메시지와 공유 타입

- 태스크 경계를 넘는 메시지는 `App/messages/<domain>_messages.h`에 정의한다.
- 입력 태스크 내부로 들어오는 메시지는 `input_messages.h`에 정의한다.
- 상태 관리 태스크가 받는 이벤트는 `state_messages.h`에 정의한다.
- 표시 태스크가 받는 명령은 `display_messages.h`에 정의한다.
- 여러 메시지 헤더를 한 번에 제공하는 집계 헤더는 `app_messages.h`로 작성한다.
- 메시지 헤더에는 payload 타입, 메시지 종류 식별자와 envelope 타입만 둔다.
- 특정 태스크의 처리 함수나 저장소 선언은 메시지 헤더에 두지 않는다.
- 여러 모듈이 공유하는 값 타입은 `App/types/<type_name>.h`에 둔다.
- 타입에 독립적인 동작 구현이 필요한 경우 같은 기본 이름의 `.c` 파일을 추가한다.
- 식별자 타입 파일은 `<subject>_id.h` 이름을 사용한다.
- 특정 기능의 설정 배열이나 런타임 singleton은 `App/types/`에 두지 않는다.

## 10. CMake와 include

- 새 `.c` 파일을 추가하면 `App/CMakeLists.txt`의 `target_sources(loopstation_app PRIVATE ...)`에 등록한다.
- 새 헤더 폴더를 include 검색 경로로 사용할 때만 `target_include_directories()`에 해당 폴더를 추가한다.
- 내부 구현 전용 헤더만 있는 폴더를 불필요하게 전역 공개 include 경로로 추가하지 않는다.
- App 코드는 가능한 한 등록된 include 경로를 사용하고 `../../../` 형태의 상대 include를 작성하지 않는다.
- App target이 STM32 HAL과 CMSIS-RTOS 타입을 공개 계약에 사용하므로 필요한 의존성은 `target_link_libraries()`로 연결한다.
- `App/CMakeLists.txt`와 루트 `CMakeLists.txt`에 같은 App 소스를 중복 등록하지 않는다.

## 11. 배치 판단 순서

- 파일이 어떤 태스크 또는 기능의 동작을 구현하는지 먼저 확인한다.
- 소유 태스크가 하나이면 해당 태스크 폴더 또는 적절한 하위 폴더에 둔다.
- 여러 파일이 참조한다는 이유만으로 공통 폴더로 이동하지 않는다.
- 여러 참조자가 같은 태스크 책임 안에 있으면 해당 태스크가 계속 소유한다.
- 둘 이상의 태스크가 사용하는 것이 타입인지, 메시지인지, 설정인지 구분한다.
- 공유 대상이 타입이면 `App/types/`, 태스크 간 전달 계약이면 `App/messages/`에 둔다.
- 둘 이상의 태스크가 같은 정적 설정을 직접 해석해야 할 때만 애플리케이션 공통 설정으로 분류한다.
- 소유권을 결정할 수 없는 파일은 공통 폴더로 이동하기 전에 책임과 변경 주체를 먼저 정의한다.
