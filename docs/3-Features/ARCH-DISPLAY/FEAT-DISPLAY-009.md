---
title: FX 표시 snapshot 수신
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 FX 표시 snapshot 수신 기능 초안을 작성함
---
# FEAT-DISPLAY-009: FX 표시 snapshot 수신
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-DISPLAY-001` | IFX와 TFX의 활성화 상태를 하나의 FX 표시 snapshot으로 표현한다. |
| `ARCH-DISPLAY-002` | 최신 FX 상태가 표시되도록 snapshot 전달 경로를 둔다. |
## 2. 설명
IFX와 TFX의 최신 활성화 상태를 하나의 표시 snapshot으로 수신한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| IFX·TFX 활성화 상태 | 상태 관리 구조가 소유하는 최신 FX 상태 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| FX 표시 snapshot | FX LED renderer의 입력 |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 최신값 | 오래된 중간 상태보다 가장 최근 snapshot을 우선한다. |
| 책임 제한 | FX 활성화 여부를 표시 구조가 변경하지 않는다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | FX 표시 snapshot 타입과 전달 경로가 없다. |
| 검증 | 미완료 | - | 구현 후 연속 toggle의 최종 표시를 검증한다. |
