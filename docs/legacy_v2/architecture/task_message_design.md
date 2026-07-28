---
title: RTOS 태스크 메시지 설계 인덱스
version: 0.11.1
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: RTOS 태스크 사이에서 오갈 메시지 후보 목록 초안을 작성함
  - date: 2026-07-08
    version: 0.2.0
    summary: 버튼 long/repeat 및 엔코더 push를 별도 메시지로 두지 않고 상태 관리 태스크에서 해석하도록 정리함
  - date: 2026-07-08
    version: 0.3.0
    summary: 표시용 오디오 telemetry를 상태 관리 태스크 라우팅 없이 디스플레이 태스크로 직접 전달하도록 정리함
  - date: 2026-07-08
    version: 0.4.0
    summary: 저장 장치 처리 결과를 요청자와 상태 전이 필요 여부에 따라 직접 전달하도록 메시지 목록을 정리함
  - date: 2026-07-08
    version: 0.5.0
    summary: 녹음 종료 요청과 실제 녹음 완료를 분리하는 메시지를 추가함
  - date: 2026-07-08
    version: 0.6.0
    summary: 트랙 파일 open/read/write/close 흐름을 오디오 처리 태스크와 저장 장치 태스크 사이의 직접 통신으로 정리함
  - date: 2026-07-08
    version: 0.7.0
    summary: 트랙 제거 동작을 파일 유지 기반 트랙 데이터 초기화 메시지로 변경함
  - date: 2026-07-08
    version: 0.7.1
    summary: 오디오 쓰기 완료 수신 시 상태 관리 태스크가 미리 저장한 목표 상태로 전이함을 명시함
  - date: 2026-07-08
    version: 0.8.0
    summary: 녹음과 오버더빙 완료를 포괄하는 오디오 트랙 쓰기 완료 메시지로 변경함
  - date: 2026-07-08
    version: 0.9.0
    summary: 완료 보고 메시지를 첫 녹음 전용 AUDIO_RECORD_DONE으로 되돌리고 오버더빙 완료는 명령으로 처리하도록 정리함
  - date: 2026-07-08
    version: 0.9.1
    summary: TEMPO_SET 설명을 보강하고 녹음 종료 frame 계산 책임을 오디오 처리 태스크로 정리함
  - date: 2026-07-08
    version: 0.10.0
    summary: 태스크별 메시지 스키마 문서를 분리하고 트랙 loop 구간 변경 메시지를 제외함
  - date: 2026-07-10
    version: 0.11.0
    summary: LED/디스플레이 표시 mailbox 분리 결정을 메시지 인덱스에 반영함
  - date: 2026-07-10
    version: 0.11.1
    summary: 트랙 파일 read-write open 정책 확정에 맞춰 후속 결정 항목을 정리함
---

# RTOS 태스크 메시지 설계 인덱스

이 문서는 RTOS 태스크/메시지 구조의 전체 방향과 태스크별 상세 메시지 문서의 위치를 정리한다.

태스크 우선순위, 큐 길이, 세마포어, 뮤텍스, 버퍼 소유권은 태스크별 메시지 문서를 바탕으로 후속 설계에서 확정한다.

참고 문서:

| 문서 | 참고 내용 |
| --- | --- |
| [software_architecture.md](./software_architecture.md) | RTOS 태스크 구성, 태스크 간 통신, 오디오 처리 구조 |
| [layered_architecture.md](./layered_architecture.md) | 사용자 이벤트, 외부 데이터, 계층별 책임 |
| [user_scenarios.md](../planning/user_scenarios.md) | 녹음, 재생, 오버더빙, 삭제, FX 조작, UI 탐색 흐름 |
| [audio_data_format.md](../audio/audio_data_format.md) | audio block, storage chunk, 내부 sample format |
| [ui_design.md](../ui/ui_design.md) | 버튼 동작, LCD 패널, LED 표시 정책 |

## 1. 수신 태스크별 메시지 문서

상태 관리 태스크는 상태 원본을 소유하고, 상태 변경이 실제 처리 결과에 영향을 주는 경우 필요한 태스크에 변경값을 전달한다. 오디오 처리 태스크는 오디오 처리에 필요한 값만 local runtime snapshot으로 보관한다.

상태 전이나 정책 판단에 필요하지 않은 비동기 처리 결과는 루프스테이션 상태 관리 태스크를 거치지 않고 요청한 태스크로 직접 반환한다. 루프스테이션 상태 관리 태스크는 사용자 입력 해석, canonical state 변경, 오디오 처리 태스크가 의미를 정리해 보고한 완료/실패 이벤트만 수신한다.

수신 방식은 다음 기준으로 구분한다.

| 수신 방식 | 의미 |
| --- | --- |
| 일반 queue | 순서가 중요하거나 누락되면 안 되는 command/event를 전달한다. |
| overwritable queue(mailbox) | 최신값만 중요하고 중간값은 버려도 되는 표시/telemetry 정보를 전달한다. |
| TODO | 아직 RTOS 기능과 구현 방식을 확정하지 않은 항목이다. |

### 1.1 태스크별 상세 스키마 문서

메시지 타입, payload 필드, 공통 envelope, queue 이름은 아래 태스크별 문서를 기준으로 한다. 이 문서의 표는 전체 흐름을 파악하기 위한 요약이다.

| 수신 태스크 | 상세 문서 | 주 queue |
| --- | --- | --- |
| 루프스테이션 상태 관리 태스크 | [state_task_messages.md](./messages/state_task_messages.md) | `state_event_queue` |
| 오디오 처리 태스크 | [audio_task_messages.md](./messages/audio_task_messages.md) | `audio_command_queue`, `audio_dma_event_queue` |
| 저장 장치 입출력 태스크 | [storage_task_messages.md](./messages/storage_task_messages.md) | `storage_request_queue` |
| LED/디스플레이 처리 태스크 | [display_task_messages.md](./messages/display_task_messages.md) | `display_command_queue`, `track_state_mailbox`, `track_param_mailbox`, `fx_state_mailbox`, `fx_param_mailbox`, `system_mailbox`, `diagnostic_mailbox`, `track_position_mailbox`, `level_meter_mailbox` |
| 사용자 컨트롤 처리 태스크 | [user_control_task_messages.md](./messages/user_control_task_messages.md) | 현재 수신 queue 없음 |
| 전체 태스크 대상 메시지 | [system_broadcast_messages.md](./messages/system_broadcast_messages.md) | TODO |

비슷한 성격의 메시지는 가능한 한 같은 queue와 공통 payload union을 공유한다. 예를 들어 상태 관리 태스크가 수신하는 사용자 입력은 `state_event_queue`의 `StateEventMessage`로, 오디오 처리 태스크가 수신하는 command/config/result는 `audio_command_queue`의 `AudioMessage`로 정규화한다.

### 1.2 문서 관리 규칙

태스크별 메시지 목록을 수정할 때는 이 파일의 링크 표가 아니라 `docs/architecture/messages/` 하위의 해당 수신 태스크 문서를 수정한다.

표시용 오디오 telemetry는 루프스테이션 상태 관리 태스크를 라우팅 경로로 사용하지 않는다. 상태 전이 판단에 트랙 위치가 필요해지는 경우에는 `AUDIO_TRACK_POSITION`을 재사용하지 않고, 의미가 명확한 별도 상태 이벤트를 정의한다.

## 2. 후속 설계에서 결정할 내용

- `TODO`로 남긴 시스템 초기화/정지 요청과 오디오 DMA 이벤트의 수신 방식을 확정한다.
- `AUDIO_RX_BLOCK_READY`, `AUDIO_TX_BLOCK_READY`처럼 실시간성이 높은 이벤트는 후속 오디오 DMA 설계에서 별도로 확정한다.
- `STORAGE_WRITE_CHUNK_REQ`, `STORAGE_READ_CHUNK_READY`, `STORAGE_CHUNK_IO_FAILED`는 buffer pointer를 포함하므로 buffer pool과 소유권 이전 규칙을 함께 정의한다.
- LED/디스플레이 처리 태스크의 표시 갱신 mailbox별 큐 길이와 overwrite 정책을 구현 단계에서 확정한다.
- 오류 메시지는 향후 [오류 처리 정책 문서](./error_handling_policy.md)에서 severity, 복구 가능 여부, 사용자 표시 정책과 연결한다.
- 트랙 파일 open/read/write/reset 정책은 향후 [트랙 파일 포맷/저장 정책 문서](../storage/track_file_storage_policy.md)에서 파일명, 디렉터리, metadata, split 정책과 함께 확정한다.
- 트랙 파일 open은 read-write 모드 단일 handle 정책을 따른다. 녹음 종료 후 재생 전환 지연, RAM prebuffer, 첫 read chunk 준비 정책은 [녹음 후 재생 전환 정책 TBA](../storage/record_to_playback_transition_tba.md)에서 별도로 검토한다.
