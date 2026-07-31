---
title: FX LED 출력
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 FX LED 출력 기능 초안을 작성함
  - date: 2026-07-31
    version: 0.2.0
    summary: DisplayTask가 표시 주기에 최신 FX snapshot을 LED에 반영하도록 변경함
---
# FEAT-DISPLAY-010: FX LED 출력
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-DISPLAY-003` | 표시 주기에 최신 FX 활성화 여부를 해당 LED의 on/off 값으로 변환한다. |
## 2. 설명
DisplayTask가 표시 주기마다 최신 FX 표시 snapshot의 활성화 상태를 IFX와 TFX LED의 on/off 출력으로 변환한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| FX 표시 snapshot | IFX·TFX 활성화 상태 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| IFX·TFX LED 출력값 | 각 LED의 켜짐 또는 꺼짐 |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 매핑 | enabled는 on, disabled는 off로 일관되게 변환한다. |
| 실행 주체 | snapshot 취득과 LED 쓰기는 DisplayTask가 수행한다. |
| 변경 최소화 | 출력값이 같으면 불필요한 하드웨어 쓰기를 생략할 수 있다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | FX LED renderer와 하드웨어 출력 경로가 없다. |
| 검증 | 미완료 | - | IFX·TFX 활성화 조합을 모두 검증한다. |
