---
title: 표시 snapshot 정의
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 UI 렌더링 payload 기능 초안을 작성함
  - date: 2026-07-31
    version: 0.2.0
    summary: UI와 LED 최신 값을 함께 제공하는 DisplaySnapshot 기능으로 변경함
---
# FEAT-DISPLAY-001: 표시 snapshot 정의
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-DISPLAY-005` | 현재 패널과 표시값을 자기완결형 `DisplaySnapshot.ui_state`로 제공한다. |
| `ARCH-DISPLAY-010` | 현재 패널의 전체 frame에 필요한 바인딩 파라미터만 snapshot에 포함한다. |
## 2. 설명
현재 `UiPanelId`, 해당 패널의 모든 `Parameter`, FX와 트랙 LED 상태를 하나의 `DisplaySnapshot`으로 표현한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| `UiPanelId`, `Parameter[]`, FX·트랙 상태 | 현재 패널과 바인딩된 표시값 및 LED 상태 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| `DisplaySnapshot` | DisplayTask가 별도 canonical 저장소 없이 사용할 최신 표시 데이터 |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 자기완결성 | panel ID와 유효 파라미터 및 LED 상태만으로 LCD와 LED 출력을 만들 수 있어야 한다. |
| 범위 | 다른 패널의 파라미터는 포함하지 않는다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | `DisplaySnapshot`과 고정 길이 `Parameter[4]`는 있으나 LED 값과 유효 파라미터 개수 계약이 없다. |
| 검증 | 미완료 | - | 파라미터 수가 다른 패널 payload를 검증해야 한다. |
