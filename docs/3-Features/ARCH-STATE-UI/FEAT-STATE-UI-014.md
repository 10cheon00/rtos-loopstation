---
title: 하위 패널 snapshot 갱신
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 하위 패널 전이 후 렌더링 기능 초안을 작성함
  - date: 2026-07-31
    version: 0.2.0
    summary: 전이 후 렌더링 요청을 최신 하위 패널 snapshot 갱신 기능으로 변경함
---
# FEAT-STATE-UI-014: 하위 패널 snapshot 갱신
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-STATE-UI-014` | StateTask가 하위 패널 전이 완료 후 새 패널 값으로 최신 UI 표시 snapshot을 덮어쓴다. |
## 2. 설명
StateTask가 Enter 전이를 마친 뒤 하위 panel ID와 바인딩 파라미터를 최신 `DisplaySnapshot.ui_state`에 게시한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| 하위 `UiPanelId`, 패널-파라미터 binding, 파라미터 저장소 | 전이된 하위 패널과 현재 표시값 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| 갱신된 `DisplaySnapshot.ui_state` | DisplayTask가 다음 표시 주기에 사용할 하위 패널 snapshot |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 후처리 | 상태 전이와 context 갱신이 끝난 뒤 snapshot을 게시한다. |
| snapshot | 새 패널에 바인딩된 값만 포함한다. |
| 출력 분리 | snapshot 게시 함수는 renderer나 LCD·LED 출력 함수를 호출하지 않는다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | 공통 snapshot 게시 경로는 있으나 하위 패널 상태와 binding이 없다. |
| 검증 | 미완료 | - | 각 하위 패널의 최초 표시값을 검증한다. |
