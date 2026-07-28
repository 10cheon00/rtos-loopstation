---
title: 입력 이벤트 큐
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 입력 이벤트 큐 기능 초안을 작성함
---

# FEAT-INPUT-003: 입력 이벤트 큐

## 1. 연결된 상위 설계 항목

| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-INPUT-001` | 좌, 우, Enter, Exit 버튼의 물리 상태를 읽는다. |
| `ARCH-INPUT-006` | IFX/TFX 버튼의 상태 변화를 공통 버튼 입력 경로로 감지한다. |
| `ARCH-INPUT-026` | 엔코더 누름 버튼 상태를 공통 버튼 입력 경로로 읽는다. |

## 2. 설명

ISR에서 생성된 `InputEvent`를 `InputTask`가 실행될 때까지 순서대로 보관한다.
큐는 버튼 인터럽트와 엔코더 회전처럼 입력 종류가 다른 이벤트를 하나의 계약으로 전달한다.

## 3. 입력

| 입력 | 설명 |
| --- | --- |
| `InputEvent` | `InputEventType`과 종류별 payload를 가진 입력 이벤트 |
| `input_event_queueHandle` | `sizeof(InputEvent)`를 원소 크기로 생성한 CMSIS-RTOS2 메시지 큐 |

## 4. 출력

| 출력 | 설명 |
| --- | --- |
| `InputEvent` | `InputTask_Run()`이 발생 순서대로 꺼낸 입력 이벤트 |
| `osStatus_t` | `osMessageQueuePut()` 또는 `osMessageQueueGet()`의 실행 결과 |

## 5. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 큐 원소 타입 | 큐 원소 크기는 `sizeof(InputEvent)`와 일치해야 한다. |
| 순서 | 동일 우선순위로 기록된 이벤트는 큐의 선입선출 순서로 소비한다. |
| 대기 | `InputTask_Run()`은 `osWaitForever`로 대기하고 이벤트 수신 후에만 처리한다. |
| 큐 포화 | `osMessageQueuePut()` 실패 여부를 확인해 입력 유실을 기록할 수 있어야 한다. |

## 6. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | 큐 생성, ISR 전송, 태스크 수신은 있으나 큐 포화 기록이 없다. |
| 검증 | 부분 완료 | 2026-07-17 | MCP23017 인터럽트 이벤트의 enqueue와 dequeue를 확인했다. |
