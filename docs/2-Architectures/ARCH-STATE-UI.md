---
title: UI 상태 머신 아키텍처 간소화 초안
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: UI 상태 머신의 공통 구조와 상태 전이 및 개발 기능만 남긴 간소화 초안을 작성함
  - date: 2026-07-28
    version: 0.1.1
    summary: 문서에 명시된 UI 상태 기능 명세의 작성 완료를 반영함
  - date: 2026-07-31
    version: 0.2.0
    summary: StateTask의 렌더링 요청을 제거하고 최신 UI snapshot 게시와 DisplayTask 주기 출력 구조를 반영함
---

# UI 상태 머신 아키텍처 간소화 초안

이 문서는 `REQ-STATE-UI` 요구사항을 만족시키기 위한 UI 상태 머신 구조를 정의한다.
UI 상태 머신은 현재 패널과 패널 탐색에 필요한 선택 context를 유지하고, `StateEvent`를 처리해 상태 전이 결과를 반환한다.
UI 상태는 LCD를 직접 그리거나 출력 시점을 결정하지 않는다.
StateTask는 이벤트 처리를 마친 뒤 현재 UI 상태와 바인딩 파라미터로 최신 `DisplaySnapshot`을 갱신할 뿐 출력 요청을 생성하지 않으며, DisplayTask가 정해진 표시 주기에 snapshot을 읽어 출력한다.

## 1. 공통 설계

| 설계 ID | 설계 설명 | 입력 | 출력 | 적용 요구사항 |
| --- | --- | --- | --- | --- |
| `ARCH-STATE-UI-001` | 각 UI 패널을 고유한 `UiPanelId`를 가진 정적 상태로 등록하고 현재 패널을 UI 상태 머신이 소유한다. | 등록된 UI 상태 목록, 초기 상태 | 현재 `UiPanelId` | `REQ-STATE-UI-001` ~ `REQ-STATE-UI-013` |
| `ARCH-STATE-UI-002` | 패널 탐색에 필요한 선택 항목, 선택 트랙, 선택 FX bus를 UI 상태 머신 context에 보관한다. | 선택 변경 이벤트 | 최신 UI 선택 context | `REQ-STATE-UI-008` ~ `REQ-STATE-UI-013` |
| `ARCH-STATE-UI-024` | 각 UI 상태는 이벤트 처리와 다음 상태 식별자 결정만 담당한다. | `StateEvent`, 현재 UI context | `StateOnEventResultFlags`, 다음 `StateId` | `REQ-STATE-UI-001` ~ `REQ-STATE-UI-013` |
| `ARCH-STATE-UI-025` | StateTask는 하나의 이벤트 처리를 모두 마친 뒤 현재 패널과 바인딩 파라미터를 최신 UI 표시 snapshot에 반영하되 출력 시점은 결정하지 않는다. | 현재 `UiPanelId`, 바인딩 파라미터 | 갱신된 `DisplaySnapshot.ui_state` | `REQ-STATE-UI-001` ~ `REQ-STATE-UI-013` |

## 2. 요구사항별 설계

### 2.1 REQ-STATE-UI-001

시스템이 정상 실행 상태에 진입하면 UI 상태 머신을 `UI_PANEL_ID_HOME`에서 시작하고 최신 UI 표시 snapshot을 홈 패널로 초기화한다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-STATE-UI-003` | UI 상태 머신의 초기 상태를 홈 패널 상태로 등록한다. | UI 상태 머신 초기화 | `UI_PANEL_ID_HOME` |
| `ARCH-STATE-UI-004` | 초기 상태 진입이 끝나면 StateTask가 홈 패널의 표시값으로 최신 UI 표시 snapshot을 초기화한다. | 현재 홈 패널, 홈 패널 바인딩 파라미터 | 초기 `DisplaySnapshot.ui_state` |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-STATE-UI-001` ~ `FEAT-STATE-UI-004` | UI 상태 모델, context, 초기 홈 상태와 최초 UI 표시 snapshot을 구현한다. |

### 2.2 REQ-STATE-UI-002 ~ REQ-STATE-UI-007

적용 요구사항: `REQ-STATE-UI-002`, `REQ-STATE-UI-003`, `REQ-STATE-UI-004`, `REQ-STATE-UI-005`, `REQ-STATE-UI-006`, `REQ-STATE-UI-007`

상위 패널을 `SETTING`, `HOME`, `FX`, `TRACK` 순서의 비순환 목록으로 두고 좌우 조작으로 인접 패널에 전이한다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-STATE-UI-005` | 상위 패널의 좌우 인접 관계를 정적 탐색표로 정의한다. | 현재 상위 `UiPanelId` | 좌우 목적 `UiPanelId` |
| `ARCH-STATE-UI-006` | 좌 조작을 `HOME -> SETTING`, `FX -> HOME`, `TRACK -> FX` 전이로 변환한다. | 좌 버튼 `StateEvent`, 현재 패널 | 다음 `StateId`와 transition flag |
| `ARCH-STATE-UI-007` | 우 조작을 `SETTING -> HOME`, `HOME -> FX`, `FX -> TRACK` 전이로 변환한다. | 우 버튼 `StateEvent`, 현재 패널 | 다음 `StateId`와 transition flag |
| `ARCH-STATE-UI-008` | 목록 경계 밖으로 이동하는 조작은 처리하되 현재 상태를 유지한다. | 경계 패널의 바깥 방향 조작 | 상태 유지 처리 결과 |
| `ARCH-STATE-UI-009` | StateTask가 상위 패널 전이 완료 후 새 패널과 바인딩 파라미터로 최신 UI 표시 snapshot을 덮어쓴다. | 새 `UiPanelId`, 바인딩 파라미터 | 갱신된 `DisplaySnapshot.ui_state` |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-STATE-UI-005` ~ `FEAT-STATE-UI-009` | 상위 패널 순서, 좌우 전이, 탐색 경계와 전이 후 UI 표시 snapshot 갱신을 구현한다. |

### 2.3 REQ-STATE-UI-008 ~ REQ-STATE-UI-010

적용 요구사항: `REQ-STATE-UI-008`, `REQ-STATE-UI-009`, `REQ-STATE-UI-010`

Enter 조작이 발생하면 현재 상위 패널과 선택 context를 사용해 진입할 하위 패널을 결정한다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-STATE-UI-010` | 현재 패널과 선택 context를 Enter 전이표에 적용한다. | Enter 버튼 `StateEvent`, 현재 패널, 선택 context | 하위 패널 `StateId` |
| `ARCH-STATE-UI-011` | 설정 패널의 선택 항목에 따라 시스템 설정 또는 하드웨어 점검 패널로 전이한다. | `SETTING`, 선택 항목 | `SYSTEM_SETTING` 또는 `HARDWARE_DIAGNOSTIC` |
| `ARCH-STATE-UI-012` | FX 패널의 선택 bus에 따라 IFX 또는 TFX 설정 패널로 전이한다. | `FX`, 선택 FX bus | `IFX_SETTING` 또는 `TFX_SETTING` |
| `ARCH-STATE-UI-013` | 트랙 패널에서 선택 트랙을 유지한 채 트랙 설정 패널로 전이한다. | `TRACK`, 선택 트랙 | `TRACK_SETTING` |
| `ARCH-STATE-UI-014` | StateTask가 하위 패널 전이 완료 후 새 패널과 바인딩 파라미터로 최신 UI 표시 snapshot을 덮어쓴다. | 하위 `UiPanelId`, 바인딩 파라미터 | 갱신된 `DisplaySnapshot.ui_state` |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-STATE-UI-010` ~ `FEAT-STATE-UI-014` | Enter 대상 선택, 하위 패널 전이와 전이 후 UI 표시 snapshot 갱신을 구현한다. |

### 2.4 REQ-STATE-UI-011

하위 패널에서 Exit 조작이 발생하면 각 하위 패널에 등록된 parent 패널로 복귀하고 기존 선택 context를 유지한다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-STATE-UI-015` | 하위 패널별 parent 패널을 정적 관계로 정의한다. | 하위 `UiPanelId` | parent `UiPanelId` |
| `ARCH-STATE-UI-016` | Exit 조작을 parent 패널 전이로 변환한다. | Exit 버튼 `StateEvent`, 현재 하위 패널 | parent `StateId`와 transition flag |
| `ARCH-STATE-UI-017` | 하위 패널 진입 전 선택했던 상위 패널 context를 복귀 후에도 유지한다. | 현재 선택 context | 유지된 선택 context |
| `ARCH-STATE-UI-018` | StateTask가 parent 패널 복귀 완료 후 해당 패널과 유지된 표시값으로 최신 UI 표시 snapshot을 덮어쓴다. | parent `UiPanelId`, 바인딩 파라미터 | 갱신된 `DisplaySnapshot.ui_state` |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-STATE-UI-015` ~ `FEAT-STATE-UI-018` | parent 관계, Exit 전이, 선택 context 유지와 복귀 후 UI 표시 snapshot 갱신을 구현한다. |

### 2.5 REQ-STATE-UI-012 ~ REQ-STATE-UI-013

IFX 또는 TFX 조작은 현재 패널과 관계없이 대응하는 FX 설정 패널로 전이하는 전역 UI 이벤트로 처리한다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-STATE-UI-019` | IFX·TFX 직접 진입 이벤트를 일반 패널 탐색보다 먼저 검사한다. | IFX 또는 TFX 버튼 `StateEvent` | 직접 진입 처리 결과 |
| `ARCH-STATE-UI-020` | IFX 조작 시 선택 FX bus를 IFX로 갱신하고 IFX 설정 패널로 전이한다. | IFX 버튼 이벤트 | `IFX_SETTING`과 transition flag |
| `ARCH-STATE-UI-021` | TFX 조작 시 선택 FX bus를 TFX로 갱신하고 TFX 설정 패널로 전이한다. | TFX 버튼 이벤트 | `TFX_SETTING`과 transition flag |
| `ARCH-STATE-UI-022` | 직접 진입한 IFX·TFX 설정 패널의 parent를 FX 패널로 유지한다. | FX 설정 `UiPanelId` | `FX` parent panel |
| `ARCH-STATE-UI-023` | StateTask가 직접 진입 완료 후 선택된 FX 설정 패널과 바인딩 파라미터로 최신 UI 표시 snapshot을 덮어쓴다. | FX 설정 `UiPanelId`, 바인딩 파라미터 | 갱신된 `DisplaySnapshot.ui_state` |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-STATE-UI-019` ~ `FEAT-STATE-UI-023` | FX 직접 진입 판정, IFX·TFX 전이, parent 관계와 UI 표시 snapshot 갱신을 구현한다. |
