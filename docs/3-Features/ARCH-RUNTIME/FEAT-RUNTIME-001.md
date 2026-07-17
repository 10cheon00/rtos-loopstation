---
title: 패널 상태 갱신 및 표시 요청
version: 0.2.0
change_history:
  - date: 2026-07-13
    version: 0.1.0
    summary: 상태 관리 태스크의 패널 상태 갱신 및 표시 요청 기능 문서를 작성함
  - date: 2026-07-17
    version: 0.2.0
    summary: 기능 구현 및 검증 상태 표를 추가함
---

# 패널 상태 갱신 및 표시 요청

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-RUNTIME-001` |
| 상위 설계 문서 | `ARCH-RUNTIME.md`, `ARCH-INPUT.md`, `ARCH-DISPLAY.md` |
| 관련 설계 항목 | `ARCH-RUNTIME-008`, `ARCH-RUNTIME-010`, `ARCH-RUNTIME-020`, `ARCH-INPUT-004`, `ARCH-INPUT-036`, `ARCH-DISPLAY-005`, `ARCH-DISPLAY-019` |
| 주요 목적 | 상태 관리 태스크가 패널 조작 버튼 입력을 현재 UI 상태에 반영하고, 디스플레이 태스크에 출력할 패널을 요청한다. |
| 제외 범위 | 버튼 debounce, 버튼 event 생성, LCD renderer 구현, LCD frame commit, FX/트랙/오디오 상태 변경 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-RUNTIME-008` | 입력 event queue | 사용자 컨트롤 처리 태스크가 보낸 `CONTROL_BUTTON`을 상태 관리 태스크가 순서대로 수신한다. |
| `ARCH-RUNTIME-010` | 상태 변경 시작 | 입력 해석 후 canonical UI state를 갱신하고 필요한 command를 생성한다. |
| `ARCH-RUNTIME-020` | canonical state | 현재 패널, 선택 트랙, 선택 항목의 원본 상태를 상태 관리 태스크가 소유한다. |
| `ARCH-INPUT-004` | 패널 탐색 해석 | 좌, 우, Enter, Exit 버튼을 현재 패널 상태 기준의 패널 이동 명령으로 해석한다. |
| `ARCH-INPUT-036` | 상태 관리 해석 | `CONTROL_BUTTON`을 시스템 동작으로 변환하는 책임이 상태 관리 태스크에 있다. |
| `ARCH-DISPLAY-005` | UI 상태 command | 갱신된 패널 상태를 `UI_STATE_RENDER` command payload로 표현한다. |
| `ARCH-DISPLAY-019` | `display_command_queue` | 패널 전환 command를 디스플레이 태스크가 순서대로 처리할 수 있게 전달한다. |

## 3. 목적

사용자 컨트롤 처리 태스크는 버튼의 물리 입력을 안정화해 `CONTROL_BUTTON` message로 상태 관리 태스크에 전달한다.
이 기능은 상태 관리 태스크가 해당 message를 현재 UI context에 맞게 해석하고, 패널 상태가 변경된 경우 canonical UI state를 먼저 갱신한 뒤 디스플레이 태스크에 새 패널 출력을 요청하도록 정의한다.

입력 감지와 표시 렌더링의 세부 흐름은 각각 `ARCH-INPUT.md`의 `REQ-INPUT-001` 설계와 `ARCH-DISPLAY.md`의 `REQ-DISPLAY-002` 설계를 참조한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| `CONTROL_BUTTON` message | `state_event_queue`에서 수신한 버튼 event |
| `ControlButtonPayload.button_id` | `BUTTON_LEFT`, `BUTTON_RIGHT`, `BUTTON_ENTER`, `BUTTON_EXIT` 중 하나인 패널 조작 버튼 |
| `ControlButtonPayload.state` | `PRESSED` 또는 `RELEASED` |
| current UI state | 현재 `panel_id`, `selected_track`, `selected_item`을 포함하는 canonical UI state |
| panel navigation table | 현재 패널과 버튼 조합에 따른 다음 패널 또는 선택 항목 전이 규칙 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| updated UI state | 상태 관리 태스크가 보관하는 갱신된 canonical UI state |
| `UI_STATE_RENDER` command | 디스플레이 태스크에 새 패널 출력을 요청하는 command |
| `UiStateRenderPayload` | `panel_id`, `selected_track`, `selected_item`, `timestamp_ms`를 포함하는 표시 요청 payload |
| enqueue result | `display_command_queue` 전송 성공 또는 실패 결과 |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 입력 선별 | 패널 조작 버튼 ID만 이 기능에서 처리하고, 다른 버튼 event는 후속 상태 해석 기능으로 넘긴다. |
| edge 정책 | 패널 전환은 `PRESSED` event 기준으로 처리하고, `RELEASED` event는 panel state를 변경하지 않는다. |
| 상태 전이 | 현재 `panel_id`와 버튼 ID를 panel navigation table에 적용해 다음 `panel_id`, `selected_track`, `selected_item`을 결정한다. |
| canonical state 갱신 | `UI_STATE_RENDER` command를 만들기 전에 상태 관리 태스크의 current UI state를 먼저 갱신한다. |
| 변경 없음 처리 | 계산된 UI state가 기존 상태와 같으면 중복 `UI_STATE_RENDER` command를 보내지 않는다. |
| 표시 요청 생성 | 패널 상태가 바뀐 경우 `ARCH-DISPLAY-005`의 `UiStateRenderPayload` 형식으로 command를 만든다. |
| queue 전송 | 생성한 command는 `display_command_queue`에 일반 queue message로 전송한다. |
| 오류 처리 | 알 수 없는 panel/button 조합, invalid panel id, queue 전송 실패는 diagnostic counter 또는 runtime error event로 기록한다. |
| 책임 분리 | 이 기능은 어떤 패널을 출력할지만 결정하며, LCD frame 생성과 실제 출력은 디스플레이 태스크가 수행한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| 버튼 해석 | `BUTTON_LEFT`, `BUTTON_RIGHT`, `BUTTON_ENTER`, `BUTTON_EXIT` press가 현재 UI state 기준의 예상 panel state로 변환되는지 확인한다. |
| release 무시 | 같은 버튼의 release event가 panel state를 변경하지 않는지 확인한다. |
| 상태 선갱신 | `UI_STATE_RENDER` 전송 전에 canonical UI state가 새 값으로 갱신되는지 확인한다. |
| 표시 요청 | panel state가 변경되면 `display_command_queue`에 `UI_STATE_RENDER` command가 들어가는지 확인한다. |
| 중복 억제 | panel state가 바뀌지 않는 입력에서 불필요한 표시 command가 생성되지 않는지 확인한다. |
| 오류 기록 | invalid panel/button 조합과 queue 전송 실패가 진단 가능한 값으로 기록되는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미확인 | - | 이번 구현 판정 범위에 포함하지 않았다. |
| 검증 | 미확인 | - | 구현 확인 후 기록한다. |
