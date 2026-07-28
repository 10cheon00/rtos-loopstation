---
title: MCP23017 인터럽트 기록
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 MCP23017 인터럽트 기록 기능 초안을 작성함
---

# FEAT-INPUT-002: MCP23017 인터럽트 기록

## 1. 연결된 상위 설계 항목

| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-INPUT-001` | 좌, 우, Enter, Exit 버튼의 물리 상태를 읽는다. |
| `ARCH-INPUT-006` | IFX/TFX 버튼의 상태 변화를 공통 버튼 입력 경로로 감지한다. |
| `ARCH-INPUT-026` | 엔코더 누름 버튼 상태를 공통 버튼 입력 경로로 읽는다. |

## 2. 설명

MCP23017의 INT 신호로 EXTI가 발생하면 ISR에서 인터럽트 출처와 발생 시각만 기록한다.
I2C 통신과 버튼 판정은 `InputTask`에서 수행한다.

## 3. 입력

| 입력 | 설명 |
| --- | --- |
| `uint16_t GPIO_Pin` | `HAL_GPIO_EXTI_Callback()`이 전달한 EXTI 핀 |
| `osKernelGetTickCount()` | 인터럽트가 감지된 RTOS tick |

## 4. 출력

| 출력 | 설명 |
| --- | --- |
| `Mcp23017IntEvent` | `gpio_pin`과 `timestamp_tick`을 담은 인터럽트 기록 |
| `InputEvent` | `type`이 `INPUT_EVENT_MCP23017`인 입력 이벤트 |

## 5. 구현 기준

| 항목 | 기준 |
| --- | --- |
| ISR 작업 | `Mcp23017IntEvent` 생성과 `input_event_queue` 전송만 수행한다. |
| 금지 작업 | ISR에서 I2C 통신, debounce, `ControlButtonId` 변환을 수행하지 않는다. |
| 대기 시간 | ISR의 `osMessageQueuePut()`은 대기하지 않도록 timeout을 0으로 사용한다. |
| 시간 기록 | 이벤트 발생 시각은 `TickType_t timestamp_tick`에 저장한다. |

## 6. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 완료 | 2026-07-17 | `HAL_GPIO_EXTI_Callback()`이 `InputEvent`를 생성해 `input_event_queue`로 전송한다. |
| 검증 | 완료 | 2026-07-17 | INTA 발생 후 이벤트가 `InputTask`에 전달되는 경로를 확인했다. |
