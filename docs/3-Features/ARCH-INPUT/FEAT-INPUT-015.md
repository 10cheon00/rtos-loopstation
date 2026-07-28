---
title: 엔코더 인터럽트 기록
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 엔코더 인터럽트 기록 기능 초안을 작성함
---

# FEAT-INPUT-015: 엔코더 인터럽트 기록

## 1. 연결된 상위 설계 항목

| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-INPUT-020` | 엔코더 입력에서 회전 방향과 이동한 칸 수를 계산한다. |
| `ARCH-INPUT-034` | 엔코더 회전 방향과 이동량을 계산해 하나의 회전 이벤트로 만든다. |

## 2. 설명

엔코더 타이머 인터럽트가 발생하면 ISR에서 카운터, 엔코더 식별자와 감지 시각만 `InputEvent`로 기록한다.

## 3. 입력

| 입력 | 설명 |
| --- | --- |
| `TIM_HandleTypeDef` | 인터럽트를 발생시킨 엔코더 타이머 |
| 타이머 counter | 인터럽트 시점의 엔코더 카운터 값 |
| `osKernelGetTickCount()` | 회전 감지 시각 |

## 4. 출력

| 출력 | 설명 |
| --- | --- |
| `EncoderRotationEvent` | `encoder_counter`, `encoder_id`, `timestamp_tick`을 담은 ISR 이벤트 |
| `InputEvent` | `type`이 `INPUT_EVENT_ENCODER_ROTATION`인 입력 이벤트 |

## 5. 구현 기준

| 항목 | 기준 |
| --- | --- |
| ISR 작업 | 카운터 snapshot, 식별자·tick 기록과 `input_event_queue` 전송만 수행한다. |
| 대기 시간 | ISR의 `osMessageQueuePut()`은 timeout 0으로 호출한다. |
| 식별자 | 여러 엔코더를 구분할 수 있는 `encoder_id`를 설정에서 가져온다. |
| 실패 처리 | 큐 전송 실패를 ISR-safe 진단 값에 기록한다. |

## 6. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | 타이머 callback의 이벤트 생성과 큐 전송은 있으나 식별자 설정과 실패 기록이 없다. |
| 검증 | 미완료 | - | 양방향 회전 시 ISR 이벤트와 큐 포화 처리를 검증한다. |
