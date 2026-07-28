---
title: 표시 및 피드백 아키텍처 간소화 초안
version: 0.1.1
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 공통 표시 구조와 요구사항별 설계 및 개발 기능만 남긴 간소화 초안을 작성함
  - date: 2026-07-28
    version: 0.1.1
    summary: 문서에 명시된 표시 기능 명세의 작성 완료를 반영함
---

# 표시 및 피드백 아키텍처 간소화 초안

이 문서는 `REQ-DISPLAY` 요구사항을 만족시키기 위한 표시 및 피드백 구조를 정의한다.
표시 구조는 전달받은 `DisplayCommand` 또는 상태 snapshot을 LCD와 LED 출력으로 변환한다.
표시할 상태의 판단과 원본 값의 소유는 상태 관리 구조가 담당하며, 표시 구조는 canonical state를 직접 변경하거나 해석하지 않는다.

## 1. 공통 설계

| 설계 ID | 설계 설명 | 입력 | 출력 | 적용 요구사항 |
| --- | --- | --- | --- | --- |
| `ARCH-DISPLAY-018` | LCD와 LED에 필요한 표시 요청을 표시 구조의 입력 계약으로 전달한다. | `DisplayCommand`, FX·트랙 상태 snapshot | LCD frame, LED 출력값 | `REQ-DISPLAY-001` ~ `REQ-DISPLAY-004` |
| `ARCH-DISPLAY-019` | 순서가 중요한 표시 명령을 `display_command_queue`에 보관하고 발생 순서대로 처리한다. | `DisplayCommand` | queue에 저장된 표시 명령 | `REQ-DISPLAY-002`, `REQ-DISPLAY-003` |
| `ARCH-DISPLAY-028` | 패널 전이와 표시 파라미터 변경 시 현재 패널의 전체 frame을 다시 생성한다. | `UiStateRenderPayload` | 현재 패널의 전체 LCD frame | `REQ-DISPLAY-002`, `REQ-DISPLAY-003` |
| `ARCH-DISPLAY-029` | 중간 변화의 순서보다 최신 상태가 중요한 LED snapshot은 최신 값만 표시 구조에 제공한다. | FX·트랙 상태 snapshot | 최신 LED 렌더링 입력 | `REQ-DISPLAY-001`, `REQ-DISPLAY-004` |
| `ARCH-DISPLAY-030` | 표시 구조의 책임을 전달받은 값의 렌더링과 출력으로 제한한다. | 표시 명령 또는 snapshot | LCD·LED 출력 | `REQ-DISPLAY-001` ~ `REQ-DISPLAY-004` |

## 2. 요구사항별 설계

### 2.1 REQ-DISPLAY-001

FX 활성화 상태를 최신 snapshot으로 받아 IFX와 TFX 표시등의 켜짐 또는 꺼짐으로 변환한다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-DISPLAY-001` | IFX와 TFX의 활성화 상태를 하나의 FX 표시 snapshot으로 표현한다. | 최신 IFX·TFX 활성화 상태 | FX 상태 snapshot |
| `ARCH-DISPLAY-002` | FX 상태의 중간 변화보다 최신 상태가 표시되도록 snapshot 전달 경로를 둔다. | FX 상태 snapshot | 최신 FX 표시 요청 |
| `ARCH-DISPLAY-003` | FX 활성화 여부를 해당 LED의 on/off 값으로 변환한다. | 최신 FX 표시 요청 | IFX·TFX LED 출력값 |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-DISPLAY-009` ~ `FEAT-DISPLAY-010` | FX 상태 snapshot 수신과 IFX·TFX LED 출력을 구현한다. |

### 2.2 REQ-DISPLAY-002

상태 관리 구조가 선택한 `UiPanelId`와 해당 패널에 바인딩된 모든 `Parameter`를 받아 선택된 패널의 전체 화면을 출력한다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-DISPLAY-005` | 현재 패널과 표시값을 자기완결형 `UiStateRenderPayload`로 전달한다. | `UiPanelId`, 현재 패널의 바인딩 파라미터 | `UiStateRenderPayload` |
| `ARCH-DISPLAY-006` | `DISPLAY_COMMAND_UI_STATE_RENDER`를 `display_command_queue`에 저장한다. | `DisplayCommand` | queue에 저장된 UI 렌더링 명령 |
| `ARCH-DISPLAY-007` | `UiPanelId`에 맞는 패널 renderer를 선택한다. | `UiStateRenderPayload.panel_id` | 패널 renderer 호출 |
| `ARCH-DISPLAY-008` | payload의 파라미터만 사용해 선택된 패널의 화면을 완성한다. | `UiStateRenderPayload.parameter` | 선택된 패널의 전체 LCD frame |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-DISPLAY-001` ~ `FEAT-DISPLAY-007` | UI render payload 정의, queue 처리, renderer 선택, frame 생성과 LCD 출력을 구현한다. |

### 2.3 REQ-DISPLAY-003

현재 패널에 표시되는 값이 바뀌면 상태 관리 구조가 이벤트 처리를 마친 후 새 `UiStateRenderPayload`를 전송하고, 표시 구조는 현재 패널 전체를 다시 그린다.
표시 구조는 다른 패널의 값이나 canonical 파라미터 저장소를 별도로 최신화하지 않는다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-DISPLAY-009` | 상태 전이 또는 파라미터 변경 결과가 있을 때 렌더링 요청을 생성한다. | `STATE_ON_EVENT_HANDLING_FLAG_TRANSITION`, `STATE_ON_EVENT_HANDLING_FLAG_PARAMETER_UPDATED` | UI 렌더링 요청 |
| `ARCH-DISPLAY-010` | 현재 패널의 전체 frame에 필요한 바인딩 파라미터만 payload에 포함한다. | 현재 `UiPanelId`, 패널-파라미터 binding, 파라미터 저장소 | 완성된 `UiStateRenderPayload` |
| `ARCH-DISPLAY-011` | 패널 전이 시 새 패널 식별자와 초기 표시값을 하나의 payload로 전달한다. | 전이된 `UiPanelId`, 새 패널의 파라미터 | 새 패널 렌더링 명령 |
| `ARCH-DISPLAY-012` | 표시 구조가 canonical 파라미터 저장소에 접근하지 않고 command만으로 frame을 생성한다. | `UiStateRenderPayload` | 독립적으로 생성된 LCD frame |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-DISPLAY-001` ~ `FEAT-DISPLAY-007` | 공통 UI 패널 출력 파이프라인을 구현한다. |
| `FEAT-DISPLAY-008` | 파라미터 변경 또는 상태 전이 후 현재 패널의 전체 frame 재생성을 요청한다. |

### 2.4 REQ-DISPLAY-004

모든 트랙의 최신 상태를 하나의 snapshot으로 받아 각 트랙 상태에 대응하는 LED 색으로 변환한다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-DISPLAY-014` | 모든 트랙의 상태를 하나의 트랙 표시 snapshot으로 표현한다. | 트랙별 현재 상태 | 트랙 상태 snapshot |
| `ARCH-DISPLAY-015` | 트랙 상태의 중간 변화보다 최신 상태가 표시되도록 snapshot 전달 경로를 둔다. | 트랙 상태 snapshot | 최신 트랙 표시 요청 |
| `ARCH-DISPLAY-016` | 각 트랙 상태를 정해진 LED 색상으로 변환한다. | 트랙별 상태 | 트랙별 LED 색상 출력값 |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-DISPLAY-011` ~ `FEAT-DISPLAY-012` | 트랙 상태 snapshot 수신과 상태별 LED 색상 출력을 구현한다. |
