---
title: UI 표시 명령 queue 전송
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 UI 표시 명령 queue 전송 기능 초안을 작성함
---
# FEAT-DISPLAY-002: UI 표시 명령 queue 전송
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-DISPLAY-006` | `DISPLAY_COMMAND_UI_STATE_RENDER`를 `display_command_queue`에 저장한다. |
| `ARCH-DISPLAY-019` | 순서가 중요한 표시 명령을 queue에 보관하고 발생 순서대로 처리한다. |
## 2. 설명
`UiStateRenderPayload`를 `DisplayCommand`에 담아 `display_command_queue`로 전송한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| `UiStateRenderPayload`, `osMessageQueueId_t` | 렌더링 payload와 표시 명령 queue |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| `DisplayCommand`, `osStatus_t` | queue에 저장할 명령과 전송 결과 |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 명령 종류 | `type`을 `DISPLAY_COMMAND_UI_STATE_RENDER`로 설정한다. |
| 실패 처리 | `osMessageQueuePut()` 실패를 호출자에게 반환한다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 완료 | 2026-07-28 | StateTask가 명령을 만들고 전송 결과를 검사한다. |
| 검증 | 미완료 | - | 연속 명령 순서와 queue 포화를 검증해야 한다. |
