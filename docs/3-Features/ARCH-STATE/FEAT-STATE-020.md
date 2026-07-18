---
title: 실패 시 canonical state 보존
version: 0.1.0
change_history:
  - date: 2026-07-18
    version: 0.1.0
    summary: 실패 시 canonical state 보존 기능 명세를 작성함
---

# 실패 시 canonical state 보존

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-STATE-020` |
| 상위 설계 문서 | `ARCH-STATE.md` |
| 관련 설계 항목 | `ARCH-STATE-020` |
| 관련 요구사항 | `REQ-STATE-006` |
| 주요 목적 | IGNORED, REJECTED, ERROR에서 마지막으로 확정된 canonical state를 유지한다. |
| 제외 범위 | commit 이후 on_enter 실패의 복구 정책 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-STATE-020` | 상태 보존 | IGNORED, REJECTED, ERROR에서 마지막으로 확정된 canonical state를 유지한다. |

## 3. 목적

IGNORED, REJECTED, ERROR에서 마지막으로 확정된 canonical state를 유지한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| 상태 처리 결과 | 무시, 거부 또는 오류 |
| 처리 전 상태 snapshot | 보존 기준 |
| 상태 머신 인스턴스 | canonical state 소유자 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| 유지된 현재 상태 | 처리 전과 같은 참조 |
| 처리 기록 | 결과 종류와 이벤트 정보 |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 상태 쓰기 금지 | 세 결과 경로에서 commit API를 호출하지 않는다. |
| context 제한 | 검증 및 분류 오류는 context도 바꾸지 않는다. |
| 결과 구분 | 세 결과를 하나의 오류로 합치지 않는다. |
| 검사 가능성 | 처리 전후 상태를 비교할 수 있어야 한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| IGNORED 보존 | 무관 이벤트 후 상태를 확인한다. |
| REJECTED 보존 | 비허용 이벤트 후 상태를 확인한다. |
| ERROR 보존 | commit 전 오류 후 상태와 기록을 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | 기능 명세에 따른 구현이 필요하다. |
| 검증 | 미검증 | - | 구현 완료 후 완료 기준에 따라 검증한다. |

