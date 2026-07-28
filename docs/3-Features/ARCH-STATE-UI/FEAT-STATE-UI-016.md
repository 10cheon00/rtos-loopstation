---
title: 하위 패널 Exit 전이
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 하위 패널 Exit 전이 기능 초안을 작성함
---
# FEAT-STATE-UI-016: 하위 패널 Exit 전이
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-STATE-UI-016` | Exit 조작을 parent 패널 전이로 변환한다. |
## 2. 설명
하위 상태가 Exit 버튼 이벤트를 받으면 parent 매핑의 상태 ID와 transition flag를 반환한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| Exit `StateEvent`, 현재 하위 `UiPanelId` | 복귀 전이 정보 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| parent `StateId`, transition flag | 상위 패널 복귀 결과 |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 조회 | 목적지는 parent 매핑에서 가져온다. |
| 분리 | 상태는 ID만 반환하고 전이 생명주기는 StateTask가 수행한다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | 하위 상태와 Exit 이벤트 처리가 없다. |
| 검증 | 미완료 | - | 각 하위 패널의 Exit 전이를 검증한다. |
