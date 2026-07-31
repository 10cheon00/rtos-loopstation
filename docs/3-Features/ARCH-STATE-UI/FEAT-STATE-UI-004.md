---
title: 최초 홈 패널 snapshot 초기화
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 최초 홈 패널 렌더링 조정 기능 초안을 작성함
  - date: 2026-07-31
    version: 0.2.0
    summary: 최초 렌더링 요청을 홈 패널 최신 표시 snapshot 초기화 기능으로 변경함
---
# FEAT-STATE-UI-004: 최초 홈 패널 snapshot 초기화
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-STATE-UI-004` | 초기 상태 진입 후 StateTask가 홈 패널 값으로 최신 UI 표시 snapshot을 초기화한다. |
## 2. 설명
HOME 진입 완료 후 StateTask가 홈 패널 ID와 바인딩 파라미터를 최신 `DisplaySnapshot.ui_state`에 게시한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| `UI_PANEL_ID_HOME`, 패널-파라미터 binding, 파라미터 저장소 | 초기 패널과 표시값 관계 및 현재 값 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| 초기 `DisplaySnapshot.ui_state` | DisplayTask가 최초 표시 주기에 사용할 홈 패널 snapshot |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 시점 | HOME `on_enter()` 성공 직후 최신 snapshot을 한 번 게시한다. |
| 최신값 | 기존 snapshot이 있다면 홈 패널 snapshot으로 덮어쓴다. |
| 책임 | UI 상태의 생명주기 함수는 표시 mailbox와 LCD·LED 출력에 접근하지 않는다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | StateTask가 초기 HOME snapshot을 게시하지만 mailbox overwrite와 게시 결과 검사가 없다. |
| 검증 | 미완료 | - | 시작 직후 DisplayTask가 HOME snapshot을 읽어 출력하는지 검증한다. |
