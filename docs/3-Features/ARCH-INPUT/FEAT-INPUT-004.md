---
title: MCP23017 버튼 상태 읽기
version: 0.2.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: 사용자 컨트롤 처리 태스크의 버튼 raw event dequeue 기능 문서를 작성함
  - date: 2026-07-12
    version: 0.2.0
    summary: MCP23017 INT event dequeue 후 I2C로 버튼 상태를 읽는 기능으로 수정함
---

# MCP23017 버튼 상태 읽기

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-INPUT-004` |
| 상위 설계 문서 | `ARCH-INPUT.md` |
| 관련 설계 항목 | `ARCH-INPUT-002`, `ARCH-INPUT-035` |
| 주요 목적 | `mcp23017_int_event_queue`에 쌓인 interrupt event를 읽고 I2C로 MCP23017 버튼 상태를 확인한다. |
| 제외 범위 | ISR 처리, debounce 판정, 상태 관리 구조 전송, 버튼 의미 해석 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-INPUT-002` | debounce | I2C로 읽은 raw button transition을 debounce 처리로 넘긴다. |
| `ARCH-INPUT-035` | raw event queue | ISR-to-task queue의 소비자 역할을 한다. |

## 3. 목적

사용자 컨트롤 처리 태스크가 실행 기회를 얻으면 MCP23017 INT queue에 저장된 event를 순서대로 읽는다.
각 event에 대해 태스크 context에서 I2C로 MCP23017의 `INTF`, `INTCAP`, 필요 시 `GPIO` 레지스터를 읽고, 변한 pin bit를 `ButtonId`와 raw state로 변환해 debounce 상태 기계에 전달한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| `mcp23017_int_event_queue` | ISR이 기록한 MCP23017 interrupt event queue |
| MCP23017 interrupt registers | I2C로 읽은 `INTF`, `INTCAP`, `GPIO` 값 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| raw button transition | debounce 처리로 넘길 버튼별 `ButtonId`, raw state, timestamp |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 처리 방식 | 태스크가 실행될 때 queue에 쌓인 MCP23017 INT event를 가능한 만큼 순서대로 drain한다. |
| 순서 처리 | dequeue 순서를 바꾸지 않는다. |
| I2C read | MCP23017 상태 레지스터 읽기는 ISR이 아니라 사용자 컨트롤 처리 태스크에서 수행한다. |
| pin 변환 | 읽은 pin bit는 `FEAT-INPUT-001`의 식별 테이블로 `ButtonId`에 매핑한다. |
| stale event | 너무 오래된 event도 timestamp를 기준으로 처리하며 임의로 폐기하지 않는다. |
| blocking | queue 대기 시간은 사용자 입력 응답 시간 요구사항을 만족하도록 설정한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| queue drain | 쌓인 MCP23017 INT event가 태스크 실행 시 순서대로 처리되는지 확인한다. |
| I2C 상태 읽기 | 태스크 context에서 `INTF`, `INTCAP` 또는 `GPIO`를 읽는지 확인한다. |
| event 전달 | 읽은 pin bit가 `ButtonId`와 raw state로 변환되어 debounce 입력으로 전달되는지 확인한다. |
