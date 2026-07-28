---
title: 우 조작 전이
version: 1.0.0
change_history:
  - date: 2026-07-18
    version: 0.1.0
    summary: 우 조작 전이 기능 명세를 작성함
  - date: 2026-07-18
    version: 0.1.1
    summary: 우 조작 전이 표현의 누락된 백틱을 수정함
  - date: 2026-07-26
    version: 1.0.0
    summary: UI 상태 머신 명칭과 식별자 변경 및 StateTask 렌더링 책임을 반영함
---

# 우 조작 전이

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-STATE-UI-007` |
| 상위 설계 문서 | `ARCH-STATE-UI.md` |
| 관련 설계 항목 | `ARCH-STATE-UI-007` |
| 관련 요구사항 | `REQ-STATE-UI-002, REQ-STATE-UI-004, REQ-STATE-UI-006` |
| 주요 목적 | 현재 패널의 오른쪽 인접 패널을 선택한다. |
| 제외 범위 | LEFT 조작과 하위 패널 전이 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-STATE-UI-007` | 우 조작 전이 | `SETTING -> HOME`, `HOME -> FX`, `FX -> TRACK`을 정의한다. |

## 3. 목적

현재 패널의 오른쪽 인접 패널을 선택한다.
이를 위해 `SETTING -> HOME`, `HOME -> FX`, `FX -> TRACK`을 정의한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| 현재 상위 panel | SETTING, HOME 또는 FX |
| RIGHT 이벤트 | 우 조작 입력 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| 상태 처리 결과 | TRANSITION(right panel) 또는 HANDLED |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 설계 결정 | `SETTING -> HOME`, `HOME -> FX`, `FX -> TRACK`을 정의한다. |
| 우 전이표 | SETTING은 HOME, HOME은 FX, FX는 TRACK으로 매핑해야 한다. |
| 오류 처리 | 입력, panel state 또는 context가 유효하지 않으면 canonical panel state를 변경하지 않고 명시적 오류 또는 거부 결과를 반환해야 한다. |
| 실행 문맥 | 상태 관리 태스크의 이벤트 처리 경로에서 동적 메모리 할당과 무제한 blocking 없이 실행해야 한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| 우 전이 | 세 출발 panel에서 기대한 목적 panel을 반환하는지 확인한다. |
| 오류 경로 | 잘못된 입력에서 panel state와 유효한 context가 훼손되지 않는지 확인한다. |
| 설계 추적 | 구현과 시험이 `ARCH-STATE-UI-007`의 설계 결정을 만족하는지 검토한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | 기능 명세에 따른 구현이 필요하다. |
| 검증 | 미검증 | - | 구현 완료 후 완료 기준에 따라 검증한다. |
