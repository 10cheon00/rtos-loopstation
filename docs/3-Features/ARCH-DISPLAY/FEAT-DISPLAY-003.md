---
title: 표시 명령 queue 수신
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 표시 명령 queue 수신 기능 초안을 작성함
---
# FEAT-DISPLAY-003: 표시 명령 queue 수신
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-DISPLAY-006` | `DISPLAY_COMMAND_UI_STATE_RENDER`를 `display_command_queue`에 저장한다. |
| `ARCH-DISPLAY-019` | 순서가 중요한 표시 명령을 발생 순서대로 처리한다. |
## 2. 설명
DisplayTask가 `display_command_queue`에서 `DisplayCommand`를 순서대로 꺼내 종류별 처리기로 전달한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| `osMessageQueueId_t display_command_queue` | 표시 명령 queue |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| `DisplayCommand` | dequeue된 표시 명령 |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 대기 | `osWaitForever`로 대기하고 수신 성공 시에만 dispatch한다. |
| dispatch | `DisplayCommandType`에 맞는 처리기를 호출한다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | queue 수신과 UI 명령 dispatch는 있으나 `osMessageQueueGet()` 결과를 검사하지 않는다. |
| 검증 | 미완료 | - | 수신 실패와 연속 명령 처리를 검증해야 한다. |
