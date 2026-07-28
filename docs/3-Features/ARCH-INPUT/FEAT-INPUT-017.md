---
title: 엔코더 회전 이벤트 전송
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 엔코더 회전 이벤트 전송 기능 초안을 작성함
---

# FEAT-INPUT-017: 엔코더 회전 이벤트 전송

## 1. 연결된 상위 설계 항목

| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-INPUT-024` | 엔코더 식별자, 회전 변화량, 이동한 칸 수, 보조 버튼 상태와 감지 시각을 하나의 이벤트로 만든다. |
| `ARCH-INPUT-035` | 변환된 입력 이벤트를 발생 순서대로 상태 관리 구조에 전달한다. |

## 2. 설명

엔코더 식별자, signed `delta`, `modifier_mask`와 감지 시각을 회전 `StateEvent`로 만들어 `state_event_queue`에 전송한다.

## 3. 입력

| 입력 | 설명 |
| --- | --- |
| `EncoderRotationEvent` | 원본 `encoder_id`와 `timestamp_tick` |
| `int32_t delta` | 방향과 이동량을 나타내는 부호 있는 변화량 |
| `modifier_mask` | 회전 시점의 보조 버튼 상태 |
| `osMessageQueueId_t state_event_queue` | 상태 관리 태스크의 입력 큐 |

## 4. 출력

| 출력 | 설명 |
| --- | --- |
| `EncoderRotationPayload` | 엔코더 회전 정보를 담은 payload |
| `StateEvent` | `type`이 `STATE_EVENT_ENCODER_ROTATION`인 이벤트 |
| `osStatus_t` | `state_event_queue` 전송 결과 |

현재 `EncoderRotationPayload`에는 `modifier_mask`가 없으므로 이벤트 계약 보완이 필요하다.

## 5. 구현 기준

| 항목 | 기준 |
| --- | --- |
| payload | `encoder_id`, signed `delta`, `modifier_mask`, 감지 시각을 하나의 payload에 저장한다. |
| 이벤트 형식 | `StateEvent.type`을 `STATE_EVENT_ENCODER_ROTATION`으로 설정한다. |
| 전송 | 완성된 이벤트를 `state_event_queue`에 발생 순서대로 보낸다. |
| 실패 처리 | `osMessageQueuePut()` 반환값을 확인하고 실패를 진단 정보로 남긴다. |
| 책임 제한 | 회전으로 수정할 파라미터와 modifier 배율을 결정하지 않는다. |

## 6. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | `EncoderRotationPayload`와 큐 전송은 있으나 modifier와 전송 실패 처리가 없다. |
| 검증 | 미완료 | - | payload 필드, 이벤트 순서와 큐 포화 처리를 검증한다. |
