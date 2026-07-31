---
title: 표시 및 피드백 아키텍처 간소화 초안
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 공통 표시 구조와 요구사항별 설계 및 개발 기능만 남긴 간소화 초안을 작성함
  - date: 2026-07-28
    version: 0.1.1
    summary: 문서에 명시된 표시 기능 명세의 작성 완료를 반영함
  - date: 2026-07-31
    version: 0.2.0
    summary: 이벤트별 출력 명령을 최신 snapshot 기반 DisplayTask 주기 출력 구조로 변경함
---

# 표시 및 피드백 아키텍처 간소화 초안

이 문서는 `REQ-DISPLAY` 요구사항을 만족시키기 위한 표시 및 피드백 구조를 정의한다.
표시 구조는 최신 `DisplaySnapshot`을 정해진 표시 주기마다 읽어 LCD와 LED 출력으로 변환한다.
표시할 상태의 판단과 원본 값의 소유는 상태 관리 구조가 담당하며, 표시 구조는 canonical state를 직접 변경하거나 이벤트마다 출력 요청을 받지 않는다.
`DisplayTask`는 마지막으로 유효하게 읽은 snapshot을 보관하고, LCD frame 전송과 LED 상태 변경이 끝난 뒤 다음 표시 주기를 처리한다.

## 1. 공통 설계

| 설계 ID | 설계 설명 | 입력 | 출력 | 적용 요구사항 |
| --- | --- | --- | --- | --- |
| `ARCH-DISPLAY-018` | LCD와 LED에 필요한 최신 값을 하나의 `DisplaySnapshot` 입력 계약으로 제공한다. | UI·FX·트랙 최신 상태 | `DisplaySnapshot` | `REQ-DISPLAY-001` ~ `REQ-DISPLAY-004` |
| `ARCH-DISPLAY-019` | 중간 표시 상태를 누적하지 않고 길이 1 overwrite mailbox에 최신 `DisplaySnapshot`만 유지한다. | `DisplaySnapshot` | `display_snapshot_mailbox`의 최신 snapshot | `REQ-DISPLAY-001` ~ `REQ-DISPLAY-004` |
| `ARCH-DISPLAY-028` | `DisplayTask`가 정해진 표시 주기마다 현재 패널의 전체 frame을 다시 생성하고 전송한다. | 마지막으로 유효하게 읽은 `DisplaySnapshot` | 현재 패널의 전체 LCD frame | `REQ-DISPLAY-002`, `REQ-DISPLAY-003` |
| `ARCH-DISPLAY-029` | FX와 트랙 상태도 같은 snapshot에서 최신 값만 취득해 주기적으로 LED에 반영한다. | `DisplaySnapshot`의 FX·트랙 상태 | 최신 LED 출력값 | `REQ-DISPLAY-001`, `REQ-DISPLAY-004` |
| `ARCH-DISPLAY-030` | 출력 시점과 LCD·LED 출력 실행은 `DisplayTask`가 소유하고, 상태 관리 구조는 출력 요청을 생성하지 않는다. | 표시 주기, 최신 `DisplaySnapshot` | LCD·LED 출력 | `REQ-DISPLAY-001` ~ `REQ-DISPLAY-004` |

## 2. 요구사항별 설계

### 2.1 REQ-DISPLAY-001

정해진 표시 주기마다 최신 snapshot의 FX 활성화 상태를 읽어 IFX와 TFX 표시등의 켜짐 또는 꺼짐으로 변환한다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-DISPLAY-001` | IFX와 TFX의 활성화 상태를 하나의 FX 표시 snapshot으로 표현한다. | 최신 IFX·TFX 활성화 상태 | FX 상태 snapshot |
| `ARCH-DISPLAY-002` | FX 상태의 중간 변화를 누적하지 않고 최신 상태를 `DisplaySnapshot`에 유지한다. | FX 상태 | 최신 FX 표시 snapshot |
| `ARCH-DISPLAY-003` | 표시 주기에 최신 FX 활성화 여부를 해당 LED의 on/off 값으로 변환한다. | 최신 FX 표시 snapshot | IFX·TFX LED 출력값 |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-DISPLAY-009` ~ `FEAT-DISPLAY-010` | FX 상태 snapshot 수신과 IFX·TFX LED 출력을 구현한다. |

### 2.2 REQ-DISPLAY-002

`DisplayTask`가 표시 주기마다 최신 `DisplaySnapshot`에서 `UiPanelId`와 해당 패널에 바인딩된 모든 `Parameter`를 읽어 선택된 패널의 전체 화면을 출력한다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-DISPLAY-005` | 현재 패널과 표시값을 자기완결형 `DisplaySnapshot.ui_state`로 제공한다. | `UiPanelId`, 현재 패널의 바인딩 파라미터 | 최신 UI 표시 snapshot |
| `ARCH-DISPLAY-006` | UI·FX·트랙 표시값을 `display_snapshot_mailbox`의 최신 원소로 덮어쓴다. | `DisplaySnapshot` | 최신 snapshot 한 건 |
| `ARCH-DISPLAY-007` | 표시 주기에 snapshot의 `UiPanelId`에 맞는 패널 renderer를 선택한다. | `DisplaySnapshot.ui_state.panel_id` | 패널 renderer 호출 |
| `ARCH-DISPLAY-008` | snapshot의 파라미터만 사용해 선택된 패널의 화면을 완성한다. | `DisplaySnapshot.ui_state.parameters` | 선택된 패널의 전체 LCD frame |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-DISPLAY-001` ~ `FEAT-DISPLAY-007` | UI render payload 정의, queue 처리, renderer 선택, frame 생성과 LCD 출력을 구현한다. |

### 2.3 REQ-DISPLAY-003

현재 패널이나 표시값이 바뀌면 최신 `DisplaySnapshot`이 overwrite mailbox에 반영된다.
`DisplayTask`는 개별 상태 이벤트와 관계없이 다음 표시 주기에 최신 snapshot으로 현재 패널 전체를 다시 그리며, 다른 패널의 값이나 canonical 파라미터 저장소를 별도로 최신화하지 않는다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-DISPLAY-009` | `DisplayTask`가 상태 이벤트 발생 여부와 무관하게 정해진 표시 주기를 관리한다. | RTOS tick, 표시 주기 | 주기적 snapshot 취득과 출력 실행 |
| `ARCH-DISPLAY-010` | 현재 패널의 전체 frame에 필요한 바인딩 파라미터만 snapshot에 포함한다. | 현재 `UiPanelId`, 패널-파라미터 binding, 파라미터 저장소 | 완성된 UI 표시 snapshot |
| `ARCH-DISPLAY-011` | 패널 전이 시 새 패널 식별자와 초기 표시값으로 최신 snapshot을 덮어쓴다. | 전이된 `UiPanelId`, 새 패널의 파라미터 | 갱신된 `DisplaySnapshot` |
| `ARCH-DISPLAY-012` | 표시 구조가 canonical 파라미터 저장소에 접근하지 않고 최신 snapshot만으로 frame을 생성한다. | `DisplaySnapshot` | 독립적으로 생성된 LCD frame |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-DISPLAY-001` ~ `FEAT-DISPLAY-007` | 공통 UI 패널 출력 파이프라인을 구현한다. |
| `FEAT-DISPLAY-008` | `DisplayTask`가 표시 주기마다 최신 snapshot으로 현재 패널의 전체 frame을 재생성한다. |

### 2.4 REQ-DISPLAY-004

정해진 표시 주기마다 모든 트랙의 최신 상태를 snapshot에서 읽어 각 트랙 상태에 대응하는 LED 색으로 변환한다.

| 설계 ID | 설계 설명 | 입력 | 출력 |
| --- | --- | --- | --- |
| `ARCH-DISPLAY-014` | 모든 트랙의 상태를 하나의 트랙 표시 snapshot으로 표현한다. | 트랙별 현재 상태 | 트랙 상태 snapshot |
| `ARCH-DISPLAY-015` | 트랙 상태의 중간 변화를 누적하지 않고 최신 상태를 `DisplaySnapshot`에 유지한다. | 트랙 상태 | 최신 트랙 표시 snapshot |
| `ARCH-DISPLAY-016` | 표시 주기에 각 트랙 상태를 정해진 LED 색상으로 변환한다. | 트랙별 최신 상태 | 트랙별 LED 색상 출력값 |

#### 개발 기능

| 기능 ID | 기능 목적 |
| --- | --- |
| `FEAT-DISPLAY-011` ~ `FEAT-DISPLAY-012` | 트랙 상태 snapshot 수신과 상태별 LED 색상 출력을 구현한다. |
