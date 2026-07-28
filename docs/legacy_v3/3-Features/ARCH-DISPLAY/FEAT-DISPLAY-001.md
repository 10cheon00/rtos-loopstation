---
title: UI_STATE_RENDER command payload 정의
version: 1.0.1
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: 선택 패널 출력을 위한 UI_STATE_RENDER command payload 기능 문서를 작성함
  - date: 2026-07-17
    version: 0.2.0
    summary: 기능 구현 및 검증 상태 표를 추가함
  - date: 2026-07-26
    version: 1.0.0
    summary: 자기완결형 UI_STATE_RENDER command와 바인딩 파라미터 전달 계약을 반영함
  - date: 2026-07-28
    version: 1.0.1
    summary: 문서의 설계 및 설명 내용을 갱신함
---

# UI_STATE_RENDER command payload 정의

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-DISPLAY-001` |
| 상위 설계 문서 | `ARCH-DISPLAY.md` |
| 관련 설계 항목 | `ARCH-DISPLAY-005`, `ARCH-DISPLAY-008`, `ARCH-DISPLAY-019` |
| 주요 목적 | 현재 UI 패널을 표시 구조가 그릴 수 있도록 panel ID와 바인딩된 모든 파라미터를 포함하는 `UI_STATE_RENDER` payload를 정의한다. |
| 제외 범위 | 패널 선택 정책, 버튼 입력 해석, LCD drawing 구현 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-DISPLAY-005` | UI render command | 현재 panel ID와 바인딩된 모든 파라미터를 표시 command로 전달한다. |
| `ARCH-DISPLAY-008` | 패널 파라미터 payload | renderer가 별도 저장소 조회 없이 사용할 파라미터를 payload에 포함한다. |
| `ARCH-DISPLAY-019` | `display_command_queue` | payload는 누락되면 안 되는 표시 명령으로 queue에 들어간다. |

## 3. 목적

StateTask가 확정한 현재 UI 패널을 표시 구조가 동일하게 해석하고 별도 저장소 조회 없이 frame을 만들 수 있도록 `UI_STATE_RENDER` command와 payload 필드를 정의한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| current UI state | StateEvent 처리가 끝난 시점의 현재 panel ID |
| panel-parameter binding | 현재 패널에 연결된 `ParameterId` 목록 |
| parameter store | binding에 대응하는 최신 `Parameter` 값 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| `UI_STATE_RENDER` command | 표시 구조가 수신할 패널 전환 command |
| `UiStateRenderPayload` | `panel_id`와 패널에 바인딩된 모든 `Parameter` 값, 선택적 sequence 값을 포함하는 payload |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| command type | message type은 `UI_STATE_RENDER`로 설정한다. |
| `panel_id` | 표시할 대상 패널을 식별할 수 있어야 한다. |
| parameter 배열 | panel-parameter binding에 등록된 순서와 같은 순서로 모든 값을 복사한다. |
| 값 복사 | 파라미터 저장소 내부 포인터를 전달하지 않고 queue 원소에 값을 복사한다. |
| 불필요한 값 제외 | 현재 패널에 바인딩되지 않은 파라미터는 payload에 포함하지 않는다. |
| timestamp | command 발생 순서 추적을 위해 단조 증가 시간 또는 동등한 sequence 값을 포함할 수 있어야 한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| payload 생성 | StateTask가 StateEvent 처리 완료 후 render가 필요할 때 `UI_STATE_RENDER` payload를 만들 수 있는지 확인한다. |
| 필드 보존 | `panel_id`와 모든 바인딩 파라미터 값이 queue 전송 전후로 유지되는지 확인한다. |
| 범위 제한 | 다른 패널 또는 미바인딩 파라미터가 payload에 포함되지 않는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미확인 | - | 이번 구현 판정 범위에 포함하지 않았다. |
| 검증 | 미확인 | - | 구현 확인 후 기록한다. |
