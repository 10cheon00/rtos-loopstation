---
title: 엔코더 signed delta 계산
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 엔코더 회전 이벤트 생성 기능 초안을 작성함
  - date: 2026-07-28
    version: 0.2.0
    summary: 엔코더 입력 기능을 분할하고 wrap-around를 고려한 signed delta 계산 범위로 한정함
---

# FEAT-INPUT-011: 엔코더 signed delta 계산

## 1. 연결된 상위 설계 항목

| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-INPUT-020` | 엔코더 입력에서 회전 방향과 이동한 칸 수를 계산한다. |
| `ARCH-INPUT-021` | 시계 방향은 양수, 반시계 방향은 음수인 변화량으로 변환한다. |
| `ARCH-INPUT-034` | 엔코더 회전 방향과 이동량을 계산해 하나의 회전 이벤트로 만든다. |

## 2. 설명

현재 엔코더 카운터와 직전 카운터의 차이를 wrap-around를 고려한 signed `delta`로 변환한다.

## 3. 입력

| 입력 | 설명 |
| --- | --- |
| `EncoderRotationEvent.encoder_counter` | ISR이 기록한 현재 엔코더 카운터 |
| `InputTaskContext.previous_encoder_counter` | 직전에 처리한 엔코더 카운터 |
| 타이머 카운터 범위 | wrap-around 계산에 사용하는 최소값과 최대값 |

## 4. 출력

| 출력 | 설명 |
| --- | --- |
| `int32_t delta` | 방향과 이동량을 함께 표현하는 부호 있는 변화량 |
| 갱신된 `previous_encoder_counter` | 다음 회전 계산에 사용할 현재 카운터 snapshot |

## 5. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 방향 | 시계 방향과 반시계 방향의 부호를 시스템 전체에서 일관되게 정의한다. |
| wrap-around | 카운터 최대값과 최소값을 통과해도 실제 이동 방향과 이동량을 유지한다. |
| snapshot | 계산이 끝난 현재 카운터를 다음 계산의 이전 값으로 저장한다. |
| 0 변화 | 계산된 `delta`가 0이면 회전 이벤트 생성 대상으로 넘기지 않는다. |

## 6. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | 16-bit wrap-around delta 계산은 있으나 타이머 설정 범위 연동과 0 변화 제거가 없다. |
| 검증 | 미완료 | - | 양방향 회전과 최소·최대 경계 통과를 검증해야 한다. |
