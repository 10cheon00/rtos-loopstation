---
title: 사용자 컨트롤 처리 태스크 수신 메시지 스키마
version: 0.1.0
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: 사용자 컨트롤 처리 태스크의 수신 메시지 없음과 송신 raw event 스키마를 정리함
---

# 사용자 컨트롤 처리 태스크 수신 메시지 스키마

현재 설계에서 사용자 컨트롤 처리 태스크가 queue로 수신해야 하는 메시지는 정의하지 않는다.

사용자 컨트롤 처리 태스크는 버튼, 엔코더, 포텐셔미터의 raw event를 루프스테이션 상태 관리 태스크로 전달한다. 현재 패널이나 선택 항목에 따라 이 입력이 어떤 상태 변경으로 해석되는지는 루프스테이션 상태 관리 태스크가 담당한다.

## 1. Queue

| Queue | 방향 | 설명 |
| --- | --- | --- |
| 없음 | 수신 | 현재 설계에서 사용자 컨트롤 처리 태스크 수신 queue는 정의하지 않는다. |
| `state_event_queue` | 송신 | 사용자 컨트롤 처리 태스크가 raw control event를 상태 관리 태스크로 보낸다. |

## 2. 송신 raw event 스키마

사용자 컨트롤 처리 태스크가 송신하는 payload는 [state_task_messages.md](./state_task_messages.md)의 다음 스키마를 사용한다.

| 메시지 타입 | Payload | 설명 |
| --- | --- | --- |
| `CONTROL_BUTTON` | `ControlButtonPayload` | 버튼 press/release 이벤트 |
| `CONTROL_ENCODER_ROTATE` | `ControlEncoderPayload` | 엔코더 회전 이벤트 |
| `CONTROL_POT_CHANGE` | `ControlPotPayload` | 포텐셔미터 변경 이벤트 |

## 3. 해석 책임

| 항목 | 담당 |
| --- | --- |
| debounce | 사용자 컨트롤 처리 태스크 |
| press/release 감지 | 사용자 컨트롤 처리 태스크 |
| long press/repeat/double click 해석 | 루프스테이션 상태 관리 태스크 |
| 엔코더 push + 회전 modifier 해석 | 루프스테이션 상태 관리 태스크 |
| BPM 변경으로 해석 | 루프스테이션 상태 관리 태스크 |
| FX 파라미터 변경으로 해석 | 루프스테이션 상태 관리 태스크 |
