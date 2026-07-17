---
title: 버튼 입력 오류 및 overflow 기록
version: 0.3.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: 버튼 입력 경로의 오류와 overflow를 기록하는 기능 문서를 작성함
  - date: 2026-07-12
    version: 0.2.0
    summary: MCP23017 INT queue overflow와 I2C 상태 읽기 오류를 기록하도록 수정함
  - date: 2026-07-12
    version: 0.2.1
    summary: 상위 설계 항목명을 버튼 변경 감지 기준으로 맞춤
  - date: 2026-07-17
    version: 0.3.0
    summary: 버튼 입력 오류 및 overflow 기록의 미구현 상태를 기록함
---

# 버튼 입력 오류 및 overflow 기록

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-INPUT-009` |
| 상위 설계 문서 | `ARCH-INPUT.md` |
| 관련 설계 항목 | `ARCH-INPUT-031`, `ARCH-INPUT-035` |
| 주요 목적 | MCP23017 INT queue overflow, 알 수 없는 pin bit, I2C read 실패, 전송 실패를 진단 가능하게 기록한다. |
| 제외 범위 | 오류 표시 UI, 런타임 오류 정책 결정, 자동 복구 정책 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-INPUT-031` | 버튼 변경 감지/debounce | 입력 처리 구조에서 감지 가능한 오류를 기록한다. |
| `ARCH-INPUT-035` | raw event queue | queue overflow와 전송 실패를 기록한다. |

## 3. 목적

버튼 입력은 ISR에서 시작되므로 오류를 즉시 복구하기 어렵다.
대신 진단 가능한 counter와 상태를 남겨 런타임 오류 처리 또는 하드웨어 점검 패널에서 확인할 수 있게 한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| unknown pin bit | source 식별 테이블에 없는 MCP23017 pin bit |
| ISR queue overflow | `mcp23017_int_event_queue`에 event를 넣지 못한 상황 |
| I2C read failure | MCP23017 상태 레지스터를 읽지 못한 상황 |
| state queue send failure | `CONTROL_BUTTON`을 상태 관리 구조로 보내지 못한 상황 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| diagnostic counter | 오류 종류별 발생 횟수 |
| last error code | 마지막 버튼 입력 오류 종류 |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| ISR 안전성 | ISR에서는 counter 증가처럼 짧고 안전한 기록만 수행한다. |
| 오류 분리 | unknown pin bit, ISR queue overflow, I2C read failure, state queue send failure를 구분한다. |
| 누적 기록 | 동일 오류가 반복되면 counter를 누적한다. |
| 표시 연결 | 오류 표시 방식은 `ARCH-DISPLAY.md`와 런타임 오류 보고 기능에서 결정한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| unknown pin 기록 | 매핑되지 않은 MCP23017 pin bit 입력 시 해당 counter가 증가하는지 확인한다. |
| ISR overflow 기록 | ISR queue full 상황에서 overflow counter가 증가하는지 확인한다. |
| I2C read 실패 기록 | MCP23017 상태 읽기 실패 시 별도 counter가 증가하는지 확인한다. |
| state queue 실패 기록 | 상태 관리 queue 전송 실패가 별도 counter로 기록되는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | queue overflow, unknown pin, I2C 실패, state queue 실패를 구분하는 진단 counter가 없다. |
| 검증 | 미완료 | - | 구현 후 오류 주입으로 검증한다. |
