---
title: CONTROL_BUTTON payload 생성
version: 0.2.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: stable 버튼 edge를 CONTROL_BUTTON payload로 변환하는 기능 문서를 작성함
  - date: 2026-07-17
    version: 0.2.0
    summary: CONTROL_BUTTON payload의 구현 및 검증 상태를 추가함
---

# CONTROL_BUTTON payload 생성

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-INPUT-007` |
| 상위 설계 문서 | `ARCH-INPUT.md` |
| 관련 설계 항목 | `ARCH-INPUT-003`, `ARCH-INPUT-007`, `ARCH-INPUT-028` |
| 주요 목적 | 확정된 버튼 edge를 상태 관리 구조가 해석할 수 있는 payload로 만든다. |
| 제외 범위 | queue 전송, 버튼 의미 해석, long press/repeat 판정 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-INPUT-003` | raw button event | 제어부 버튼 edge를 `CONTROL_BUTTON` payload로 만든다. |
| `ARCH-INPUT-007` | FX button event | FX 버튼 edge도 같은 payload 형식을 사용한다. |
| `ARCH-INPUT-028` | push event 생성 | 엔코더 push edge도 같은 payload 형식을 사용한다. |

## 3. 목적

stable button edge를 상태 관리 구조가 해석할 수 있는 `CONTROL_BUTTON` message payload로 변환한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| stable button edge | 버튼 ID, 확정 상태, timestamp |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| `ControlButtonPayload` | 상태 관리 구조로 보낼 버튼 event payload |

## 6. 구현 기준

| 필드 | 기준 |
| --- | --- |
| `button_id` | stable edge의 `ButtonId`를 그대로 담는다. |
| `state` | `PRESSED` 또는 `RELEASED`를 담는다. |
| `timestamp_ms` | raw event timestamp 또는 stable edge 확정 시각 중 하나로 일관되게 정의해 사용한다. |

press와 release를 모두 payload로 만들 수 있어야 한다.
long press, repeat, modifier 해석은 상태 관리 구조 또는 후속 기능이 이 payload를 바탕으로 수행한다.

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| 필드 정확성 | `button_id`, `state`, `timestamp_ms`가 기대값으로 채워지는지 확인한다. |
| press/release 지원 | press와 release가 모두 payload로 생성되는지 확인한다. |
| 해석 비포함 | payload 생성 단계에서 패널 이동이나 FX toggle을 판단하지 않는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | `ControlButtonPayload` 생성 코드는 있으나 stable edge 입력과 timestamp 단위가 확정되지 않았다. |
| 검증 | 미완료 | - | debounce 이후 press/release payload 경로를 구현한 뒤 검증한다. |
