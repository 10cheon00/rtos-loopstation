---
title: 최신 표시 snapshot 게시
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 UI 표시 명령 queue 전송 기능 초안을 작성함
  - date: 2026-07-31
    version: 0.2.0
    summary: 순차 표시 명령 전송을 길이 1 overwrite snapshot mailbox 게시 기능으로 변경함
---
# FEAT-DISPLAY-002: 최신 표시 snapshot 게시
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-DISPLAY-006` | UI·FX·트랙 표시값을 `display_snapshot_mailbox`의 최신 원소로 덮어쓴다. |
| `ARCH-DISPLAY-019` | 길이 1 overwrite mailbox에 최신 `DisplaySnapshot`만 유지한다. |
## 2. 설명
최신 UI·FX·트랙 상태로 만든 `DisplaySnapshot`을 길이 1 `display_snapshot_mailbox`에 게시하고 기존 원소가 있으면 덮어쓴다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| `DisplaySnapshot`, `osMessageQueueId_t` | 최신 표시 snapshot과 mailbox |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| 갱신된 `display_snapshot_mailbox`, 게시 결과 | 최신 snapshot 한 건과 갱신 성공 여부 |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 최신값 우선 | mailbox 길이는 1이며 기존 snapshot이 있으면 원자적으로 덮어쓴다. |
| 비차단 | snapshot 게시 때문에 StateTask가 DisplayTask의 출력 완료를 기다리지 않는다. |
| 실패 처리 | overwrite 실패를 호출자에게 반환하거나 오류 상태로 기록한다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | 길이 1 mailbox와 snapshot 생성은 있으나 `osMessageQueuePut()`이 기존 원소를 덮어쓰지 않고 결과도 검사하지 않는다. |
| 검증 | 미완료 | - | 빠른 연속 갱신 후 mailbox에 마지막 snapshot만 남는지 검증해야 한다. |
