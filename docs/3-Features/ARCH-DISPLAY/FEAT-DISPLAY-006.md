---
title: LCD 전체 frame 생성
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 LCD 전체 frame 생성 기능 초안을 작성함
---
# FEAT-DISPLAY-006: LCD 전체 frame 생성
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-DISPLAY-008` | payload의 파라미터만 사용해 선택된 패널의 화면을 완성한다. |
| `ARCH-DISPLAY-028` | 현재 패널의 전체 frame을 다시 생성한다. |
## 2. 설명
선택된 renderer가 u8g2 buffer를 지우고 패널의 전체 레이아웃과 표시값을 다시 그린다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| `u8g2_t *`, `Parameter *` | frame buffer와 패널 표시값 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| u8g2 frame buffer, `UI_DRAWING_STATUS` | 완성된 전체 frame과 결과 |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 전체 그리기 | `u8g2_ClearBuffer()` 후 모든 요소를 다시 그린다. |
| 패널 분리 | 패널별 renderer를 독립 함수로 구현한다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | HOME과 SETTING 전체 frame renderer만 구현되어 있다. |
| 검증 | 부분 완료 | 2026-07-28 | 코드상 buffer 초기화를 확인했으며 모든 패널의 실제 출력 검증은 남아 있다. |
