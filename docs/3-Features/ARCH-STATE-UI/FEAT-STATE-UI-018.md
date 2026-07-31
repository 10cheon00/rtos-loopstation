---
title: parent 패널 snapshot 갱신
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 parent 패널 복귀 후 렌더링 기능 초안을 작성함
  - date: 2026-07-31
    version: 0.2.0
    summary: 복귀 후 렌더링 요청을 최신 parent 패널 snapshot 갱신 기능으로 변경함
---
# FEAT-STATE-UI-018: parent 패널 snapshot 갱신
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-STATE-UI-018` | StateTask가 parent 패널 복귀 완료 후 해당 패널 값으로 최신 UI 표시 snapshot을 덮어쓴다. |
## 2. 설명
StateTask가 Exit 전이를 마친 뒤 parent panel ID와 유지된 표시값을 최신 `DisplaySnapshot.ui_state`에 게시한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| parent `UiPanelId`, UI context, 바인딩 파라미터 | 복귀한 패널과 유지된 표시 정보 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| 갱신된 `DisplaySnapshot.ui_state` | DisplayTask가 다음 표시 주기에 사용할 parent 패널 snapshot |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 후처리 | Exit 전이와 context 보존이 끝난 뒤 snapshot을 게시한다. |
| 값 복원 | 유지된 선택값이 snapshot 파라미터에 반영되어야 한다. |
| 출력 분리 | snapshot 게시 함수는 renderer나 LCD·LED 출력 함수를 호출하지 않는다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | 공통 snapshot 게시 경로만 있고 Exit 전이는 없다. |
| 검증 | 미완료 | - | 복귀 화면과 선택값 복원을 검증한다. |
