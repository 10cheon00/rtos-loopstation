---
title: UI_STATE_RENDER command payload 정의
version: 0.2.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: 선택 패널 출력을 위한 UI_STATE_RENDER command payload 기능 문서를 작성함
  - date: 2026-07-17
    version: 0.2.0
    summary: 기능 구현 및 검증 상태 표를 추가함
---

# UI_STATE_RENDER command payload 정의

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-DISPLAY-001` |
| 상위 설계 문서 | `ARCH-DISPLAY.md` |
| 관련 설계 항목 | `ARCH-DISPLAY-005`, `ARCH-DISPLAY-008`, `ARCH-DISPLAY-019` |
| 주요 목적 | 선택된 패널을 표시 구조가 그릴 수 있도록 `UI_STATE_RENDER` command payload 형식을 정의한다. |
| 제외 범위 | 패널 선택 정책, 버튼 입력 해석, LCD drawing 구현 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-DISPLAY-005` | UI 상태 command | 새 panel id와 선택 항목을 표시 command로 전달한다. |
| `ARCH-DISPLAY-008` | 선택 항목 표시 | `selected_track`, `selected_item`을 payload에 포함한다. |
| `ARCH-DISPLAY-019` | `display_command_queue` | payload는 누락되면 안 되는 표시 명령으로 queue에 들어간다. |

## 3. 목적

상태 관리 구조가 선택한 패널을 표시 구조가 동일하게 해석할 수 있도록 `UI_STATE_RENDER` command와 payload 필드를 명확히 정의한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| current panel state | 상태 관리 구조가 결정한 새 패널 ID |
| selected track | 새 패널에서 강조할 트랙 |
| selected item | 새 패널에서 강조할 항목 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| `UI_STATE_RENDER` command | 표시 구조가 수신할 패널 전환 command |
| `UiStateRenderPayload` | `panel_id`, `selected_track`, `selected_item`, `timestamp_ms`를 포함하는 payload |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| command type | message type은 `UI_STATE_RENDER`로 설정한다. |
| `panel_id` | 표시할 대상 패널을 식별할 수 있어야 한다. |
| `selected_track` | 트랙 선택이 없는 패널에서는 none 또는 invalid 값을 명시적으로 표현한다. |
| `selected_item` | 선택 항목이 없는 패널에서는 none 또는 invalid 값을 명시적으로 표현한다. |
| timestamp | command 발생 순서 추적을 위해 단조 증가 시간 또는 동등한 sequence 값을 포함할 수 있어야 한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| payload 생성 | 상태 관리 구조가 패널 전환 시 `UI_STATE_RENDER` payload를 만들 수 있는지 확인한다. |
| 필드 보존 | `panel_id`, `selected_track`, `selected_item` 값이 queue 전송 전후로 유지되는지 확인한다. |
| invalid 표현 | 선택값이 없는 패널에서 none 또는 invalid 값이 일관되게 표현되는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미확인 | - | 이번 구현 판정 범위에 포함하지 않았다. |
| 검증 | 미확인 | - | 구현 확인 후 기록한다. |
