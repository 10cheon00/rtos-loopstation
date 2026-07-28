---
title: 상태 관리 event queue 전송
version: 0.2.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: CONTROL_BUTTON message를 상태 관리 event queue로 전송하는 기능 문서를 작성함
  - date: 2026-07-13
    version: 0.1.1
    summary: 상태 관리 태스크 수신 queue 이름을 state_event_queue로 명시함
  - date: 2026-07-17
    version: 0.2.0
    summary: 상태 관리 event queue 전송의 구현 및 검증 상태를 추가함
---

# 상태 관리 event queue 전송

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-INPUT-008` |
| 상위 설계 문서 | `ARCH-INPUT.md` |
| 관련 설계 항목 | `ARCH-INPUT-003`, `ARCH-INPUT-035` |
| 주요 목적 | `CONTROL_BUTTON` message를 상태 관리 구조의 `state_event_queue`로 전송한다. |
| 제외 범위 | ISR-to-task queue, debounce, 버튼 의미 해석 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-INPUT-003` | raw button event | message type은 `CONTROL_BUTTON`이다. |
| `ARCH-INPUT-035` | raw event queue | 상태 관리 구조가 수신하는 `state_event_queue`를 사용한다. |

## 3. 목적

사용자 컨트롤 처리 태스크가 생성한 `CONTROL_BUTTON` message를 상태 관리 태스크가 수신하는 `state_event_queue`로 전송한다.
상태 관리 태스크가 이 queue에서 event를 읽어 패널 상태를 갱신하는 흐름은 `FEAT-RUNTIME-001.md`를 참조한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| `ControlButtonPayload` | 확정된 버튼 event payload |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| `CONTROL_BUTTON` | `state_event_queue`에 들어가는 raw control event |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| message type | `CONTROL_BUTTON`으로 설정한다. |
| queue 대상 | 상태 관리 태스크가 수신하는 `state_event_queue`를 사용한다. |
| 순서 보존 | 사용자 컨트롤 처리 태스크가 확정한 stable edge 순서대로 전송한다. |
| 전송 실패 | 전송 실패를 진단 counter에 기록하고 런타임 오류 보고 정책과 연결한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| message 전송 | `CONTROL_BUTTON` message가 `state_event_queue`에 들어가는지 확인한다. |
| 순서 보존 | 연속 생성된 button message가 같은 순서로 수신되는지 확인한다. |
| 실패 기록 | queue 전송 실패 시 진단 counter가 증가하는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | queue 전송 호출은 있으나 `StateEvent` envelope, queue 원소 타입, 전송 실패 처리가 일치하지 않는다. |
| 검증 | 미완료 | - | 상태 관리 태스크 수신까지 메시지 계약을 맞춘 뒤 검증한다. |
