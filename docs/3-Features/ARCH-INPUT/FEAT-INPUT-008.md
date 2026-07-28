---
title: 상태 이벤트 큐 전송
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 상태 이벤트 큐 전송 기능 초안을 작성함
---

# FEAT-INPUT-008: 상태 이벤트 큐 전송

## 1. 연결된 상위 설계 항목

| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-INPUT-003` | 버튼 식별자, 상태, 감지 시각을 하나의 이벤트로 만든다. |
| `ARCH-INPUT-007` | FX 버튼 식별자와 상태를 하나의 이벤트로 만든다. |
| `ARCH-INPUT-028` | 엔코더 누름 버튼 식별자와 상태를 하나의 이벤트로 만든다. |
| `ARCH-INPUT-035` | 변환된 입력 이벤트를 발생 순서대로 상태 관리 구조에 전달한다. |

## 2. 설명

`ControlButtonPayload`를 `StateEvent`에 담아 `state_event_queue`로 전송한다.
전송 이후 이벤트의 해석과 상태 변경은 상태 관리 구조가 담당한다.

## 3. 입력

| 입력 | 설명 |
| --- | --- |
| `ControlButtonPayload` | 식별자, 상태, 시각을 가진 버튼 이벤트 payload |
| `osMessageQueueId_t state_event_queue` | 상태 관리 태스크가 수신하는 메시지 큐 |

## 4. 출력

| 출력 | 설명 |
| --- | --- |
| `StateEvent` | `type`이 `STATE_EVENT_CONTROL_BUTTON`이고 `payload.control_button`이 채워진 이벤트 |
| `osStatus_t` | `osMessageQueuePut()`의 전송 결과 |

## 5. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 이벤트 형식 | `StateEvent.type`을 `STATE_EVENT_CONTROL_BUTTON`으로 설정한다. |
| payload | `StateEvent.payload.control_button`에 입력 `ControlButtonPayload`를 저장한다. |
| 전송 순서 | 확정된 버튼 상태 변화의 발생 순서대로 `state_event_queue`에 전송한다. |
| 실패 처리 | `osMessageQueuePut()`의 반환값을 확인하고 실패를 진단 정보로 남긴다. |

## 6. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | `StateEvent` 생성과 큐 전송은 있으나 전송 실패 처리가 없다. |
| 검증 | 미완료 | - | 상태 관리 태스크의 수신과 연속 이벤트 순서를 검증해야 한다. |
