---
title: 패널 선택 context 갱신
version: 0.2.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: 패널 전환 시 표시 구조의 현재 패널 context를 갱신하는 기능 문서를 작성함
  - date: 2026-07-17
    version: 0.2.0
    summary: 기능 구현 및 검증 상태 표를 추가함
---

# 패널 선택 context 갱신

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-DISPLAY-005` |
| 상위 설계 문서 | `ARCH-DISPLAY.md` |
| 관련 설계 항목 | `ARCH-DISPLAY-005`, `ARCH-DISPLAY-008`, `ARCH-DISPLAY-012` |
| 주요 목적 | 표시 구조가 현재 출력 중인 패널과 선택 항목 context를 갱신한다. |
| 제외 범위 | 상태 관리 구조의 canonical UI state 결정, renderer drawing |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-DISPLAY-005` | UI 상태 command | 새 `panel_id`, `selected_track`, `selected_item`을 받는다. |
| `ARCH-DISPLAY-008` | 선택 항목 표시 | 현재 선택 track/item을 강조할 수 있게 context를 유지한다. |
| `ARCH-DISPLAY-012` | 표시 구조 최소 상태 | 현재 패널 context만 표시 구조의 최소 상태로 유지한다. |

## 3. 목적

표시 구조가 현재 LCD에 어떤 패널을 그리고 있는지 알 수 있도록 내부 display context를 갱신한다.
이 context는 이후 `ARCH-DISPLAY-009` 표시 대상 선별에도 사용될 수 있다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| `UiStateRenderPayload` | 새 패널 ID와 선택 항목 정보를 담은 payload |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| display panel context | 현재 표시 중인 `panel_id`, `selected_track`, `selected_item` |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 갱신 시점 | `UI_STATE_RENDER` command를 처리할 때 context를 갱신한다. |
| 최소 상태 | 표시 구조는 화면 출력에 필요한 현재 패널 context만 유지한다. |
| 선택 항목 | 선택값이 없는 패널에서는 none 또는 invalid 값을 저장한다. |
| 일관성 | renderer에 전달하는 context와 내부 저장 context가 같은 값을 사용해야 한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| panel 갱신 | `UI_STATE_RENDER` 처리 후 현재 `panel_id`가 갱신되는지 확인한다. |
| 선택값 갱신 | `selected_track`, `selected_item`이 payload와 같은 값으로 저장되는지 확인한다. |
| renderer 일치 | renderer가 받은 context와 저장된 context가 일치하는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미확인 | - | 이번 구현 판정 범위에 포함하지 않았다. |
| 검증 | 미확인 | - | 구현 확인 후 기록한다. |
