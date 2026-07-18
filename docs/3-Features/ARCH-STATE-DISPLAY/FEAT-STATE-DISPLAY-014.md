---
title: 하위 패널 render
version: 0.1.1
change_history:
  - date: 2026-07-18
    version: 0.1.0
    summary: 하위 패널 render 기능 명세를 작성함
  - date: 2026-07-18
    version: 0.1.1
    summary: 하위 패널 render 메시지 식별자의 누락된 백틱을 수정함
---

# 하위 패널 render

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-STATE-DISPLAY-014` |
| 상위 설계 문서 | `ARCH-STATE-DISPLAY.md` |
| 관련 설계 항목 | `ARCH-STATE-DISPLAY-014` |
| 관련 요구사항 | `REQ-STATE-DISPLAY-008 ~ REQ-STATE-DISPLAY-010` |
| 주요 목적 | 선택 대상과 함께 하위 패널을 출력한다. |
| 제외 범위 | 패널별 세부 표시 payload와 LCD drawing |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-STATE-DISPLAY-014` | 하위 패널 render | `UI_STATE_RENDER`에 panel id와 현재 선택 context를 포함한다. |

## 3. 목적

선택 대상과 함께 하위 패널을 출력한다.
이를 위해 `UI_STATE_RENDER`에 panel id와 현재 선택 context를 포함한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| 하위 panel 진입 결과 | commit된 child state |
| display context | 선택 항목, track 또는 FX bus |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| UI_STATE_RENDER | child panel ID와 선택 context |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 설계 결정 | `UI_STATE_RENDER`에 panel id와 현재 선택 context를 포함한다. |
| context 포함 | child panel을 그리는 데 필요한 선택 식별자를 render 요청에 포함해야 한다. |
| 오류 처리 | 입력, panel state 또는 context가 유효하지 않으면 canonical panel state를 변경하지 않고 명시적 오류 또는 거부 결과를 반환해야 한다. |
| 실행 문맥 | 상태 관리 태스크의 이벤트 처리 경로에서 동적 메모리 할당과 무제한 blocking 없이 실행해야 한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| 하위 render | 각 child panel에서 panel ID와 선택 context가 보존되는지 확인한다. |
| 오류 경로 | 잘못된 입력에서 panel state와 유효한 context가 훼손되지 않는지 확인한다. |
| 설계 추적 | 구현과 시험이 `ARCH-STATE-DISPLAY-014`의 설계 결정을 만족하는지 검토한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | 기능 명세에 따른 구현이 필요하다. |
| 검증 | 미검증 | - | 구현 완료 후 완료 기준에 따라 검증한다. |
