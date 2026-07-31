---
title: 상위 패널 snapshot 갱신
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 상위 패널 전이 후 렌더링 기능 초안을 작성함
  - date: 2026-07-31
    version: 0.2.0
    summary: 전이 후 렌더링 요청을 최신 상위 패널 snapshot 갱신 기능으로 변경함
---
# FEAT-STATE-UI-009: 상위 패널 snapshot 갱신
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-STATE-UI-009` | StateTask가 상위 패널 전이 완료 후 새 패널 값으로 최신 UI 표시 snapshot을 덮어쓴다. |
## 2. 설명
StateTask가 상위 패널 전이를 마친 뒤 current panel과 바인딩 파라미터를 최신 `DisplaySnapshot.ui_state`에 게시한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| current `UiPanelId`, 패널-파라미터 binding, 파라미터 저장소 | 전이된 패널과 현재 표시값 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| 갱신된 `DisplaySnapshot.ui_state` | DisplayTask가 다음 표시 주기에 사용할 상위 패널 snapshot |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 후처리 | `StateMachine_DoTransition()` 이후 StateTask가 최신 snapshot을 게시한다. |
| 출력 분리 | snapshot 게시 함수는 renderer나 LCD·LED 출력 함수를 호출하지 않는다. |
| 최신값 | 표시 주기 사이의 여러 전이는 중간 snapshot을 누적하지 않고 마지막 상태로 덮어쓴다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | HOME·SETTING 전이 후 snapshot 게시 경로는 있으나 overwrite가 구현되지 않았다. |
| 검증 | 부분 완료 | 2026-07-28 | 두 패널 전이와 snapshot 생성 경로를 코드로 확인했으며 연속 전이 최종값 검증이 남아 있다. |
