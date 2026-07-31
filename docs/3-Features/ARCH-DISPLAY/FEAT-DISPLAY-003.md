---
title: 주기적 표시 snapshot 취득
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 표시 명령 queue 수신 기능 초안을 작성함
  - date: 2026-07-31
    version: 0.2.0
    summary: 명령 queue blocking 수신을 DisplayTask의 주기적 최신 snapshot 취득 기능으로 변경함
---
# FEAT-DISPLAY-003: 주기적 표시 snapshot 취득
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-DISPLAY-009` | `DisplayTask`가 정해진 표시 주기를 관리한다. |
| `ARCH-DISPLAY-019` | overwrite mailbox에서 최신 `DisplaySnapshot`만 취득한다. |
## 2. 설명
DisplayTask가 정해진 표시 주기에 `display_snapshot_mailbox`를 비차단으로 읽고, 성공하면 마지막 유효 snapshot을 갱신한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| `osMessageQueueId_t display_snapshot_mailbox`, 표시 주기 | 최신 snapshot mailbox와 주기 기준 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| `DisplaySnapshot` | 이번 주기에 사용할 마지막 유효 snapshot |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 주기 | 절대 tick 기준으로 다음 실행 시점을 계산해 출력 시작 간격을 유지한다. |
| 비차단 수신 | `osMessageQueueGet(..., 0)` 성공 시에만 로컬 snapshot을 교체한다. |
| 유효값 보관 | 새 snapshot이 없으면 초기화되지 않은 값을 사용하지 않고 마지막 유효값을 유지한다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | DisplayTask의 주기 실행과 비차단 수신은 있으나 수신 결과를 검사하지 않고 마지막 유효 snapshot 여부를 관리하지 않는다. |
| 검증 | 미완료 | - | 빈 mailbox, 연속 overwrite와 표시 주기 유지 여부를 검증해야 한다. |
