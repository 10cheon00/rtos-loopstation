---
title: 사용자 컨트롤 입력 큐
version: 0.3.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: ISR 버튼 event를 사용자 컨트롤 처리 태스크로 전달하는 입력 큐 기능 문서를 작성함
  - date: 2026-07-12
    version: 0.2.0
    summary: MCP23017 INT event를 태스크로 전달하는 입력 큐로 수정함
  - date: 2026-07-12
    version: 0.2.1
    summary: 상위 설계 항목명을 버튼 변경 감지 기준으로 맞춤
  - date: 2026-07-17
    version: 0.3.0
    summary: 사용자 컨트롤 입력 큐의 구현 및 검증 상태를 추가함
---

# 사용자 컨트롤 입력 큐

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-INPUT-003` |
| 상위 설계 문서 | `ARCH-INPUT.md` |
| 관련 설계 항목 | `ARCH-INPUT-035`, `ARCH-INPUT-031` |
| 주요 목적 | ISR에서 발생한 MCP23017 INT event를 사용자 컨트롤 처리 태스크가 나중에 순서대로 읽을 수 있게 보관한다. |
| 제외 범위 | debounce, `CONTROL_BUTTON` payload 생성, 상태 관리 구조 전송 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-INPUT-035` | raw event queue | MCP23017 INT ISR과 사용자 컨트롤 처리 태스크 사이의 event 전달 경로다. |
| `ARCH-INPUT-031` | 버튼 변경 감지/debounce | ISR은 기록만 하고 태스크가 후속 처리를 수행한다. |

## 3. 목적

MCP23017 INT 라인으로 버튼 변화를 감지하더라도 사용자 컨트롤 처리 태스크가 즉시 실행되지 않을 수 있다.
이 기능은 ISR event를 queue에 누적해 interrupt 발생 사실을 보존한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| `Mcp23017IntEvent` | ISR에서 기록한 MCP23017 interrupt event |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| queued interrupt event | 사용자 컨트롤 처리 태스크가 순서대로 읽을 수 있는 event |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| queue 종류 | ISR에서 write 가능하고 사용자 컨트롤 처리 태스크에서 read 가능한 queue를 사용한다. |
| 순서 보존 | MCP23017 INT event의 enqueue 순서를 보존한다. |
| queue 길이 | 예상되는 interrupt burst와 태스크 지연 시간을 고려해 설정한다. |
| overflow | queue가 가득 차면 overflow counter를 증가시킨다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| ISR enqueue | ISR context에서 event enqueue가 가능한지 확인한다. |
| 순서 보존 | 연속 입력 event가 enqueue 순서대로 dequeue되는지 확인한다. |
| overflow 기록 | queue full 상황에서 overflow counter가 증가하는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | queue 생성, ISR enqueue, InputTask dequeue를 구현했으나 overflow counter가 없다. |
| 검증 | 부분 완료 | 2026-07-17 | INTA event가 queue를 거쳐 InputTask에서 dequeue되는 경로를 확인했다. |
