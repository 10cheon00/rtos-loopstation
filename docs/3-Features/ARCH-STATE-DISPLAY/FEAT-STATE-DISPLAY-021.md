---
title: TFX 직접 전이
version: 0.1.0
change_history:
  - date: 2026-07-18
    version: 0.1.0
    summary: TFX 직접 전이 기능 명세를 작성함
---

# TFX 직접 전이

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-STATE-DISPLAY-021` |
| 상위 설계 문서 | `ARCH-STATE-DISPLAY.md` |
| 관련 설계 항목 | `ARCH-STATE-DISPLAY-021` |
| 관련 요구사항 | `REQ-STATE-DISPLAY-013` |
| 주요 목적 | TFX 조작의 목적 패널을 고정한다. |
| 제외 범위 | TFX 활성화 상태 변경 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-STATE-DISPLAY-021` | TFX 직접 전이 | 현재 상태에서 `TFX_SETTING`으로 전이하고 `selected_fx_bus=TFX`로 갱신한다. |

## 3. 목적

TFX 조작의 목적 패널을 고정한다.
이를 위해 현재 상태에서 `TFX_SETTING`으로 전이하고 `selected_fx_bus=TFX`로 갱신한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| TFX 직접 조작 이벤트 | TFX panel 이동 요청 |
| 현재 display context | 기존 선택값 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| 상태 처리 결과 | TRANSITION(TFX_SETTING) |
| 갱신 context | selected FX bus가 TFX |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 설계 결정 | 현재 상태에서 `TFX_SETTING`으로 전이하고 `selected_fx_bus=TFX`로 갱신한다. |
| context 선행 | 전이 결과 생성 전에 selected FX bus를 TFX 의미로 확정해야 한다. |
| 오류 처리 | 입력, panel state 또는 context가 유효하지 않으면 canonical panel state를 변경하지 않고 명시적 오류 또는 거부 결과를 반환해야 한다. |
| 실행 문맥 | 상태 관리 태스크의 이벤트 처리 경로에서 동적 메모리 할당과 무제한 blocking 없이 실행해야 한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| TFX 직접 전이 | 대표 출발 panel에서 TFX_SETTING과 TFX context가 생성되는지 확인한다. |
| 오류 경로 | 잘못된 입력에서 panel state와 유효한 context가 훼손되지 않는지 확인한다. |
| 설계 추적 | 구현과 시험이 `ARCH-STATE-DISPLAY-021`의 설계 결정을 만족하는지 검토한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | 기능 명세에 따른 구현이 필요하다. |
| 검증 | 미검증 | - | 구현 완료 후 완료 기준에 따라 검증한다. |

