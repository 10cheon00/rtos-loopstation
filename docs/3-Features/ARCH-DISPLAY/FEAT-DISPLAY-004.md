---
title: 패널 renderer 선택
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 패널 renderer 선택 기능 초안을 작성함
  - date: 2026-07-31
    version: 0.2.0
    summary: 주기적으로 취득한 DisplaySnapshot의 패널 식별자를 renderer 선택 입력으로 변경함
---
# FEAT-DISPLAY-004: 패널 renderer 선택
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-DISPLAY-007` | 표시 주기에 snapshot의 `UiPanelId`에 맞는 패널 renderer를 선택한다. |
## 2. 설명
마지막으로 유효하게 취득한 `DisplaySnapshot.ui_state.panel_id`를 renderer table에 적용해 패널 renderer를 선택한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| `DisplaySnapshot.ui_state.panel_id`, renderer table | 최신 패널 식별자와 renderer 매핑 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| `Ui_UiPanelRenderFunction` | 선택된 패널 renderer |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 검증 | `panel_id < UI_PANEL_ID_COUNT`이고 함수 포인터가 NULL이 아닌지 확인한다. |
| 매핑 | 지원하는 모든 `UiPanelId`에 정확히 하나의 renderer를 등록한다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | HOME과 SETTING 매핑은 있으나 범위·NULL 검사와 나머지 패널이 없다. |
| 검증 | 미완료 | - | 유효·무효 panel ID를 검증해야 한다. |
