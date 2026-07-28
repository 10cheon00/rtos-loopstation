---
title: Exit 전이
version: 1.0.0
change_history:
  - date: 2026-07-18
    version: 0.1.0
    summary: Exit 전이 기능 명세를 작성함
  - date: 2026-07-26
    version: 1.0.0
    summary: UI 상태 머신 명칭과 식별자 변경 및 StateTask 렌더링 책임을 반영함
---

# Exit 전이

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-STATE-UI-016` |
| 상위 설계 문서 | `ARCH-STATE-UI.md` |
| 관련 설계 항목 | `ARCH-STATE-UI-016` |
| 관련 요구사항 | `REQ-STATE-UI-011` |
| 주요 목적 | 하위 패널에서 상위 패널로 돌아간다. |
| 제외 범위 | parent panel의 on_enter와 render |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-STATE-UI-016` | Exit 전이 | Exit 처리 결과로 `TRANSITION(parent_panel)`을 반환한다. |

## 3. 목적

하위 패널에서 상위 패널로 돌아간다.
이를 위해 Exit 처리 결과로 `TRANSITION(parent_panel)`을 반환한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| 현재 child panel | parent mapping이 있는 상태 |
| EXIT 이벤트 | 상위 복귀 요청 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| 상태 처리 결과 | TRANSITION(parent panel) |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 설계 결정 | Exit 처리 결과로 `TRANSITION(parent_panel)`을 반환한다. |
| child 제한 | 상위 panel에서 EXIT를 받았을 때 임의 parent 전이를 생성하면 안 된다. |
| 오류 처리 | 입력, panel state 또는 context가 유효하지 않으면 canonical panel state를 변경하지 않고 명시적 오류 또는 거부 결과를 반환해야 한다. |
| 실행 문맥 | 상태 관리 태스크의 이벤트 처리 경로에서 동적 메모리 할당과 무제한 blocking 없이 실행해야 한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| Exit 전이 | 각 child panel에서 정의된 parent를 반환하는지 확인한다. |
| 오류 경로 | 잘못된 입력에서 panel state와 유효한 context가 훼손되지 않는지 확인한다. |
| 설계 추적 | 구현과 시험이 `ARCH-STATE-UI-016`의 설계 결정을 만족하는지 검토한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | 기능 명세에 따른 구현이 필요하다. |
| 검증 | 미검증 | - | 구현 완료 후 완료 기준에 따라 검증한다. |

