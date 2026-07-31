---
title: 버튼 상태 이벤트 생성
version: 0.1.2
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 버튼 상태 이벤트 생성 기능 초안을 작성함
  - date: 2026-07-30
    version: 0.1.1
    summary: 문서 변경 사항 반영
  - date: 2026-07-31
    version: 0.1.2
    summary: 문서 변경 사항 반영
---

# FEAT-INPUT-007: 버튼 상태 이벤트 생성

## 1. 연결된 상위 설계 항목

| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-INPUT-003` | 버튼 식별자, 상태, 감지 시각을 하나의 이벤트로 만든다. |
| `ARCH-INPUT-007` | FX 버튼 식별자와 상태를 하나의 이벤트로 만든다. |
| `ARCH-INPUT-028` | 엔코더 누름 버튼 식별자와 상태를 하나의 이벤트로 만든다. |

## 2. 설명

확정된 버튼 식별자와 눌림 상태를 상태 관리 구조가 공통으로 받을 수 있는 `ControlButtonPayload`로 변환한다.
버튼에 따른 판단과 행동은 포함하지 않는다.

## 3. 입력

| 입력 | 설명 |
| --- | --- |
| `ControlButtonId` | 물리 입력에서 변환된 버튼 식별자 |
| `ControlButtonState` | 확정된 `PRESSED` 또는 `RELEASED` 상태 |
| `TickType_t timestamp_ticks` | 원본 버튼 입력이 감지된 RTOS tick |

## 4. 출력

| 출력 | 설명 |
| --- | --- |
| `ControlButtonPayload` | `id`, `state`, `timestamp_ms`를 담은 버튼 이벤트 payload |

## 5. 구현 기준

| 항목 | 기준 |
| --- | --- |
| `id` | 입력의 `ControlButtonId`를 변경하지 않고 저장한다. |
| `state` | `CONTROL_BUTTON_STATE_PRESSED` 또는 `CONTROL_BUTTON_STATE_RELEASED`를 저장한다. |
| 시간 단위 | `timestamp_ms`에 tick을 저장할지 ms로 변환할지 계약을 확정하고 필드명과 일치시킨다. |
| 책임 제한 | 패널 이동, FX 활성화, 트랙 조작처럼 버튼의 의미를 해석하지 않는다. |

## 6. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | `ControlButtonPayload` 생성은 있으나 debounce 입력과 timestamp 단위가 확정되지 않았다. |
| 검증 | 미완료 | - | debounce가 끝난 press/release 입력으로 payload 필드를 검증해야 한다. |
