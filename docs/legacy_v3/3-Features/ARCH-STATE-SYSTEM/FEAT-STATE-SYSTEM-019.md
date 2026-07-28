---
title: 종료 요청
version: 0.1.0
change_history:
  - date: 2026-07-18
    version: 0.1.0
    summary: 종료 요청 기능 명세를 작성함
---

# 종료 요청

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-STATE-SYSTEM-019` |
| 상위 설계 문서 | `ARCH-STATE-SYSTEM.md` |
| 관련 설계 항목 | `ARCH-STATE-SYSTEM-019` |
| 관련 요구사항 | `REQ-STATE-SYSTEM-008` |
| 주요 목적 | 정상 종료 시작 시점을 명시한다. |
| 제외 범위 | END.on_enter의 cleanup 요청 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-STATE-SYSTEM-019` | 종료 요청 | `SYSTEM_SHUTDOWN_REQUEST` 처리 시 `RUNNING -> END`로 전이한다. |

## 3. 목적

정상 종료 시작 시점을 명시한다.
이를 위해 `SYSTEM_SHUTDOWN_REQUEST` 처리 시 `RUNNING -> END`로 전이한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| SYSTEM_SHUTDOWN_REQUEST 이벤트 | 정상 종료 요청 |
| 현재 시스템 상태 | RUNNING |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| 상태 처리 결과 | TRANSITION(END) 또는 REJECTED |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 설계 결정 | `SYSTEM_SHUTDOWN_REQUEST` 처리 시 `RUNNING -> END`로 전이한다. |
| 출발 상태 제한 | 정상 종료 전이는 RUNNING에서만 허용해야 한다. |
| 오류 처리 | 입력이나 현재 상태가 유효하지 않으면 canonical state를 변경하지 않고 명시적 오류 또는 거부 결과를 반환해야 한다. |
| 실행 문맥 | 상태 관리 태스크의 이벤트 처리 경로에서 무제한 blocking과 동적 메모리 할당 없이 실행해야 한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| 종료 전이 | RUNNING에서는 END, 다른 상태에서는 정의된 거부 결과인지 확인한다. |
| 오류 경로 | 잘못된 입력에서 상태가 훼손되지 않고 정의된 실패 결과가 반환되는지 확인한다. |
| 설계 추적 | 구현과 시험이 `ARCH-STATE-SYSTEM-019`의 설계 결정을 만족하는지 검토한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | 기능 명세에 따른 구현이 필요하다. |
| 검증 | 미검증 | - | 구현 완료 후 완료 기준에 따라 검증한다. |

