---
title: MCP23017 INT ISR event 기록
version: 0.2.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: 버튼 EXTI ISR에서 raw event를 기록하는 기능 문서를 작성함
  - date: 2026-07-12
    version: 0.2.0
    summary: 개별 버튼 EXTI가 아닌 MCP23017 INT 라인 ISR event 기록으로 수정함
---

# MCP23017 INT ISR event 기록

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-INPUT-002` |
| 상위 설계 문서 | `ARCH-INPUT.md` |
| 관련 설계 항목 | `ARCH-INPUT-001`, `ARCH-INPUT-006`, `ARCH-INPUT-026` |
| 주요 목적 | MCP23017 INT 라인 assert를 ISR에서 최소 정보로 기록해 사용자 컨트롤 처리 태스크에 전달한다. |
| 제외 범위 | debounce, long press 판정, 버튼 의미 해석, display/audio command 생성 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-INPUT-001` | 버튼 scan | polling 대신 MCP23017 INT 라인 기반 event 기록으로 구현한다. |
| `ARCH-INPUT-006` | FX 버튼 감지 | FX 버튼 변화도 같은 MCP23017 INT 기록 경로를 사용한다. |
| `ARCH-INPUT-026` | encoder push scan | 엔코더 push 변화도 같은 MCP23017 INT 기록 경로를 사용한다. |

## 3. 목적

우선순위가 높은 태스크가 길게 실행되어도 MCP23017의 버튼 변화 알림을 놓치지 않도록, MCP23017 INT 라인이 assert되면 ISR에서 interrupt 발생 사실을 queue에 기록한다.
ISR에서는 I2C read를 수행하지 않고, 사용자 컨트롤 처리 태스크가 나중에 MCP23017의 `INTF`, `INTCAP`, `GPIO`를 읽어 어떤 버튼이 변했는지 확인한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| MCU interrupt | MCP23017 INT 핀 assert로 발생한 MCU interrupt |
| MCP23017 INT source | 어떤 MCP23017 장치 또는 INT 라인에서 interrupt가 발생했는지 나타내는 값 |
| timestamp | ISR 진입 시점 또는 INT 포착 시점 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| `Mcp23017IntEvent` | 사용자 컨트롤 처리 태스크로 전달할 ISR-safe interrupt event |

`Mcp23017IntEvent`는 `mcp23017_id`, `int_line`, `timestamp_ms`를 포함한다.
버튼별 `button_id`와 `raw_state`는 태스크 context에서 I2C 상태를 읽은 뒤 생성한다.

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| ISR 작업량 | MCP23017 INT source 식별, timestamp 기록, queue 전송만 수행한다. |
| 금지 작업 | ISR에서 I2C read, debounce, 버튼 의미 해석, display/audio command 생성을 수행하지 않는다. |
| queue 전송 | ISR-safe queue API로 `mcp23017_int_event_queue`에 event를 기록한다. |
| task wakeup | queue 전송 후 사용자 컨트롤 처리 태스크를 깨울 수 있는 RTOS 메커니즘을 사용한다. |
| 오류 처리 | queue 전송 실패는 ISR-safe 방식으로 오류 counter에 기록한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| INT 기록 | MCP23017 INT assert 시 `Mcp23017IntEvent`가 queue에 들어가는지 확인한다. |
| ISR 최소 처리 | ISR에서 I2C read, debounce, 의미 해석을 수행하지 않는지 확인한다. |
| task wakeup | queue 기록 후 사용자 컨트롤 처리 태스크가 실행 기회를 얻는지 확인한다. |
