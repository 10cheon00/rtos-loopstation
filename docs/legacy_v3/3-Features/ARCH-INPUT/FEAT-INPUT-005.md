---
title: 버튼 debounce 판정
version: 0.3.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: EXTI raw 버튼 event를 stable edge로 확정하는 debounce 기능 문서를 작성함
  - date: 2026-07-12
    version: 0.2.0
    summary: MCP23017에서 읽은 raw 버튼 transition을 debounce하도록 수정함
  - date: 2026-07-12
    version: 0.2.1
    summary: 상위 설계 항목명을 버튼 변경 감지 기준으로 맞춤
  - date: 2026-07-17
    version: 0.3.0
    summary: debounce 기능의 미구현 상태를 기록함
---

# 버튼 debounce 판정

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-INPUT-005` |
| 상위 설계 문서 | `ARCH-INPUT.md` |
| 관련 설계 항목 | `ARCH-INPUT-002`, `ARCH-INPUT-006`, `ARCH-INPUT-026` |
| 주요 목적 | MCP23017에서 읽은 raw button transition 중 bounce를 제거하고 stable press/release를 확정한다. |
| 제외 범위 | MCP23017 INT ISR 기록, I2C 상태 읽기, `CONTROL_BUTTON` queue 전송, 버튼 의미 해석 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-INPUT-002` | debounce | 제어부 버튼의 stable press/release edge를 만든다. |
| `ARCH-INPUT-006` | FX 버튼 감지 | FX 버튼도 같은 debounce 경로를 사용한다. |
| `ARCH-INPUT-026` | encoder push 변경 감지 | 엔코더 push도 같은 debounce 경로를 사용한다. |

## 3. 목적

MCP23017 INT는 bounce로 인해 반복 assert될 수 있으므로, 사용자 컨트롤 처리 태스크에서 버튼별 debounce를 수행해 stable edge만 확정한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| raw button transition | MCP23017 상태 읽기에서 얻은 `ButtonId`, raw state, timestamp |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| stable button edge | debounce 후 확정된 `PRESSED` 또는 `RELEASED` edge |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 상태 보관 | 버튼별 `last_raw_state`, `stable_state`, `last_transition_time`을 보관한다. |
| debounce 기준 | raw state가 debounce 시간 이상 유지된 경우 stable edge로 확정한다. |
| 중복 제거 | stable state가 바뀌지 않으면 `CONTROL_BUTTON`을 생성하지 않는다. |
| 설정값 | debounce 시간은 공통 기본값을 두되 버튼별 override가 가능하게 한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| bounce 제거 | bounce edge가 여러 번 들어와도 stable edge가 중복 생성되지 않는지 확인한다. |
| press/release 확정 | 실제 press와 release가 각각 stable edge로 확정되는지 확인한다. |
| 버튼별 독립성 | 한 버튼의 bounce가 다른 버튼 debounce 상태에 영향을 주지 않는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | InputTask에 버튼별 debounce 상태와 시간 판정이 없다. |
| 검증 | 미완료 | - | 구현 후 검증한다. |
