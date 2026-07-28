---
title: 직접 진입 render
version: 1.0.0
change_history:
  - date: 2026-07-18
    version: 0.1.0
    summary: 직접 진입 render 기능 명세를 작성함
  - date: 2026-07-26
    version: 1.0.0
    summary: UI 상태 머신 명칭과 식별자 변경 및 StateTask 렌더링 책임을 반영함
---

# 직접 진입 render

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-STATE-UI-023` |
| 상위 설계 문서 | `ARCH-STATE-UI.md` |
| 관련 설계 항목 | `ARCH-STATE-UI-023` |
| 관련 요구사항 | `REQ-STATE-UI-012, REQ-STATE-UI-013` |
| 주요 목적 | 선택한 FX 설정 화면을 즉시 표시한다. |
| 제외 범위 | FX 파라미터 표시 payload와 LCD drawing |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-STATE-UI-023` | 직접 진입 render 조정 | StateTask가 직접 진입 StateEvent 처리 완료 후 FX 설정 panel ID와 바인딩된 모든 파라미터로 `UI_STATE_RENDER`를 생성한다. |

## 3. 목적

선택한 FX 설정 화면을 즉시 표시한다.
UI 상태는 render command를 생성하지 않으며, StateTask가 직접 진입 전이와 StateEvent 처리를 완료한 뒤 current panel binding을 조회해 `UI_STATE_RENDER`를 생성한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| FX 직접 전이 완료 | IFX_SETTING 또는 TFX_SETTING |
| 상태 처리 결과 | 직접 진입 전이를 나타내는 `TRANSITION` flag |
| FX 설정 panel binding | 현재 FX 설정 패널에 바인딩된 모든 `ParameterId` |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| UI_STATE_RENDER | FX 설정 panel ID와 바인딩된 모든 파라미터 |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 설계 결정 | StateTask가 직접 진입 StateEvent 처리 완료 후 FX 설정 panel ID와 바인딩된 모든 파라미터로 `UI_STATE_RENDER`를 생성한다. |
| 즉시 표시 | 전이 완료 후 다른 상위 panel render를 거치지 않고 목적 설정 panel 요청을 생성해야 한다. |
| 상태 책임 제한 | IFX/TFX 설정 상태는 display queue와 파라미터 저장소를 참조하지 않는다. |
| 오류 처리 | 입력, panel state 또는 context가 유효하지 않으면 canonical panel state를 변경하지 않고 명시적 오류 또는 거부 결과를 반환해야 한다. |
| 실행 문맥 | 상태 관리 태스크의 이벤트 처리 경로에서 동적 메모리 할당과 무제한 blocking 없이 실행해야 한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| 직접 render | IFX와 TFX 각각 올바른 panel ID와 바인딩 파라미터가 전달되는지 확인한다. |
| 처리 순서 | 직접 진입 StateEvent 처리가 끝난 후 command가 enqueue되는지 확인한다. |
| 오류 경로 | 잘못된 입력에서 panel state와 유효한 context가 훼손되지 않는지 확인한다. |
| 설계 추적 | 구현과 시험이 `ARCH-STATE-UI-023`의 설계 결정을 만족하는지 검토한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | 기능 명세에 따른 구현이 필요하다. |
| 검증 | 미검증 | - | 구현 완료 후 완료 기준에 따라 검증한다. |
