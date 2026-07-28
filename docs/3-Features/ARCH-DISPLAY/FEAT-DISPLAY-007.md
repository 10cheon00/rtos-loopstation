---
title: LCD frame 출력
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 LCD frame 출력 기능 초안을 작성함
---
# FEAT-DISPLAY-007: LCD frame 출력
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-DISPLAY-018` | 표시 요청을 LCD와 LED 출력으로 변환한다. |
| `ARCH-DISPLAY-028` | 생성된 현재 패널의 전체 frame을 출력한다. |
## 2. 설명
완성된 u8g2 frame buffer를 LCD 통신 경로로 전송한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| `u8g2_t *` | 완성된 frame buffer를 가진 u8g2 context |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| LCD 화면 | `u8g2_SendBuffer()`로 반영된 전체 frame |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| commit | frame 생성이 끝난 뒤 한 번만 LCD에 전송한다. |
| 오류 | 하드웨어 전송 실패를 `UI_DRAWING_STATUS` 또는 동등한 결과로 전달한다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | renderer가 `u8g2_SendBuffer()`를 호출하지만 전송 실패 계약이 없다. |
| 검증 | 부분 완료 | 2026-07-28 | HOME과 SETTING 출력 경로를 확인했으며 연속 갱신 검증이 남아 있다. |
