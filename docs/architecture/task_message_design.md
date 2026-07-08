---
title: RTOS 태스크 메시지 후보 목록
version: 0.1.0
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: RTOS 태스크 사이에서 오갈 메시지 후보 목록 초안을 작성함
---

# RTOS 태스크 메시지 후보 목록

이 문서는 RTOS 태스크/메시지 구조를 확정하기 전에, 현재 문서들에서 확인되는 태스크 간 메시지 후보를 정리한다.

태스크 우선순위, 큐 길이, 세마포어, 뮤텍스, 버퍼 소유권은 이 문서의 메시지 목록을 바탕으로 후속 설계에서 확정한다.

참고 문서:

| 문서 | 참고 내용 |
| --- | --- |
| [software_architecture.md](./software_architecture.md) | RTOS 태스크 구성, 태스크 간 통신, 오디오 처리 구조 |
| [layered_architecture.md](./layered_architecture.md) | 사용자 이벤트, 외부 데이터, 계층별 책임 |
| [user_scenarios.md](../planning/user_scenarios.md) | 녹음, 재생, 오버더빙, 삭제, FX 조작, UI 탐색 흐름 |
| [audio_data_format.md](../audio/audio_data_format.md) | audio block, storage chunk, 내부 sample format |
| [ui_design.md](../ui/ui_design.md) | 버튼 동작, LCD 패널, LED 표시 정책 |

## 1. 수신 태스크별 메시지 후보

상태 관리 태스크는 상태 원본을 소유하고, 상태 변경이 실제 처리 결과에 영향을 주는 경우 필요한 태스크에 변경값을 전달한다. 오디오 처리 태스크는 오디오 처리에 필요한 값만 local runtime snapshot으로 보관한다.

수신 방식은 다음 기준으로 구분한다.

| 수신 방식 | 의미 |
| --- | --- |
| 일반 queue | 순서가 중요하거나 누락되면 안 되는 command/event를 전달한다. |
| overwritable queue(mailbox) | 최신값만 중요하고 중간값은 버려도 되는 표시/telemetry 정보를 전달한다. |
| TODO | 아직 RTOS 기능과 구현 방식을 확정하지 않은 항목이다. |

### 1.1 루프스테이션 상태 관리 태스크 수신 메시지

| 메시지 종류 | 송신 태스크 | 메시지 타입 | 수신 방식 | 간단한 설명 |
| --- | --- | --- | --- | --- |
| 시스템 부팅 | 시스템 초기화 흐름 | `SYSTEM_BOOT` | TODO | 전원 인가 후 상태 모델을 초기화하고 초기 화면 표시를 준비한다. |
| 버튼 입력 | 사용자 컨트롤 처리 태스크 | `CONTROL_BUTTON` | 일반 queue | 녹음/재생, 정지, IFX, TFX, 좌/우, Enter, Exit 버튼 입력을 전달한다. |
| 길게 누름 입력 | 사용자 컨트롤 처리 태스크 | `CONTROL_BUTTON_LONG` | 일반 queue | 정지 버튼 길게 누름처럼 짧은 입력과 구분해야 하는 입력을 전달한다. |
| 연속 입력 | 사용자 컨트롤 처리 태스크 | `CONTROL_BUTTON_REPEAT` | 일반 queue | 짧은 시간 안의 연속 입력을 트랙 삭제 요청 등으로 해석하기 위해 전달한다. |
| 엔코더 회전 | 사용자 컨트롤 처리 태스크 | `CONTROL_ENCODER_ROTATE` | 일반 queue | 로터리 엔코더 회전 방향과 이동량을 전달한다. |
| 엔코더 push | 사용자 컨트롤 처리 태스크 | `CONTROL_ENCODER_PUSH` | 일반 queue | 로터리 엔코더 push 입력을 선택 또는 확인 이벤트로 전달한다. |
| 포텐셔미터 값 변경 | 사용자 컨트롤 처리 태스크 | `CONTROL_POT_CHANGE` | 일반 queue | threshold/rate limit을 거친 ADC 포텐셔미터 변경값을 전달한다. |
| 트랙 위치 보고 | 오디오 처리 태스크 | `AUDIO_TRACK_POSITION` | overwritable queue(mailbox) | UI 표시나 loop 상태 판단에 필요한 현재 재생/녹음 위치를 전달한다. |
| 레벨 미터 보고 | 오디오 처리 태스크 | `AUDIO_LEVEL_METER` | overwritable queue(mailbox) | LCD 하단 볼륨 표시에 사용할 입력/출력 레벨 정보를 전달한다. |
| 오디오 dropout 보고 | 오디오 처리 태스크 | `AUDIO_DROPOUT_DETECTED` | 일반 queue | 오디오 block 처리 지연 또는 누락을 보고한다. |
| 파일 처리 결과 | 저장 장치 입출력 태스크 | `STORAGE_FILE_STATUS` | 일반 queue | 파일 open/read/write/close/delete 결과를 상태 모델에 반영한다. |
| 저장장치 상태 | 저장 장치 입출력 태스크 | `STORAGE_MEDIA_STATUS` | 일반 queue | SD 카드 삽입, mount 상태, 남은 용량 같은 저장장치 상태를 보고한다. |
| 저장장치 오류 | 저장 장치 입출력 태스크 | `STORAGE_ERROR` | 일반 queue | SD 카드, FatFs, 파일 처리 오류를 보고한다. |
| 표시 완료 보고 | LED/디스플레이 처리 태스크 | `UI_RENDER_DONE` | 일반 queue | LCD 또는 LED 갱신 완료 상태를 보고한다. 필요 없으면 생략할 수 있다. |

### 1.2 오디오 처리 태스크 수신 메시지

| 메시지 종류 | 송신 태스크 | 메시지 타입 | 수신 방식 | 간단한 설명 |
| --- | --- | --- | --- | --- |
| 입력 패스스루 설정 | 루프스테이션 상태 관리 태스크 | `AUDIO_PASSTHROUGH_ENABLE` | 일반 queue | IFX 이후의 입력 신호를 녹음 상태와 독립적으로 믹싱 경로에 보낼지 설정한다. |
| 녹음 시작 | 루프스테이션 상태 관리 태스크 | `TRACK_RECORD_START` | 일반 queue | 대상 트랙을 `RECORDING` 상태로 전환하고 입력 오디오 저장을 시작하도록 요청한다. |
| 녹음 완료 | 루프스테이션 상태 관리 태스크 | `TRACK_RECORD_FINISH` | 일반 queue | `RECORDING -> PLAYING` 전이를 위해 녹음을 확정하고 재생 가능한 트랙으로 마무리하도록 요청한다. |
| 녹음 정지 | 루프스테이션 상태 관리 태스크 | `TRACK_RECORD_STOP` | 일반 queue | 정지 버튼 입력으로 `RECORDING -> STOPPED` 전이를 수행하기 위해 녹음을 중단하도록 요청한다. |
| 재생 시작 | 루프스테이션 상태 관리 태스크 | `TRACK_PLAY_START` | 일반 queue | 저장된 트랙을 읽어 반복 재생하도록 오디오 처리 경로를 준비한다. |
| 재생 정지 | 루프스테이션 상태 관리 태스크 | `TRACK_PLAY_STOP` | 일반 queue | 정지 버튼 입력으로 `PLAYING -> STOPPED` 전이를 수행하기 위해 재생 중인 트랙을 정지한다. |
| 오버더빙 시작 | 루프스테이션 상태 관리 태스크 | `TRACK_OVERDUB_START` | 일반 queue | 기존 트랙을 재생하면서 새 입력 오디오를 합칠 준비를 요청한다. |
| 오버더빙 완료 | 루프스테이션 상태 관리 태스크 | `TRACK_OVERDUB_FINISH` | 일반 queue | `OVERDUBBING -> PLAYING` 전이를 위해 오버더빙 결과를 확정하고 재생 상태로 복귀하도록 요청한다. |
| 오버더빙 정지 | 루프스테이션 상태 관리 태스크 | `TRACK_OVERDUB_STOP` | 일반 queue | 정지 버튼 입력으로 `OVERDUBBING -> STOPPED` 전이를 수행하기 위해 오버더빙을 중단하도록 요청한다. |
| FX 활성화 설정 | 루프스테이션 상태 관리 태스크 | `FX_ENABLE_SET` | 일반 queue | IFX 또는 TFX 활성화 상태를 오디오 처리 태스크에 반영한다. |
| FX 선택 | 루프스테이션 상태 관리 태스크 | `FX_SELECT` | 일반 queue | 현재 IFX/TFX 종류를 변경한다. |
| FX 파라미터 변경 | 루프스테이션 상태 관리 태스크 | `FX_PARAM_SET` | 일반 queue | 노브, 엔코더, UI 조작으로 변경된 FX 파라미터를 전달한다. |
| 트랙 gain 변경 | 루프스테이션 상태 관리 태스크 | `TRACK_GAIN_SET` | 일반 queue | 트랙별 볼륨 또는 gain 값을 오디오 믹싱 경로에 반영한다. |
| 트랙 TFX 설정 | 루프스테이션 상태 관리 태스크 | `TRACK_TFX_SET` | 일반 queue | 트랙별 TFX 적용 여부를 오s디오 처리 경로에 반영한다. |
| 트랙 재생 방향 변경 | 루프스테이션 상태 관리 태스크 | `TRACK_PLAY_DIRECTION_SET` | 일반 queue | 트랙 재생 방향을 정방향 또는 역방향으로 설정한다. |
| 트랙 loop 구간 변경 | 루프스테이션 상태 관리 태스크 | `TRACK_LOOP_REGION_SET` | 일반 queue | 반복 재생할 loop 구간을 오디오 처리 경로에 반영한다. |
| BPM 변경 | 루프스테이션 상태 관리 태스크 | `TEMPO_SET` | 일반 queue | BPM 변경을 오디오 처리 경로에 반영한다. |
| master gain 변경 | 루프스테이션 상태 관리 태스크 | `MASTER_GAIN_SET` | 일반 queue | 믹싱 이후 최종 출력 master gain 값을 반영한다. |
| 오디오 입력 block 준비 | SAI DMA callback | `AUDIO_RX_BLOCK_READY` | TODO | SAI 수신 버퍼의 half/full block이 준비되었음을 알린다. |
| storage chunk 쓰기 완료 | 저장 장치 입출력 태스크 | `STORAGE_WRITE_CHUNK_DONE` | 일반 queue | storage chunk 쓰기 완료와 버퍼 반환 가능 여부를 알린다. |
| storage chunk 읽기 완료 | 저장 장치 입출력 태스크 | `STORAGE_READ_CHUNK_READY` | 일반 queue | 읽은 트랙 audio block 또는 storage chunk가 준비되었음을 알린다. |
| loop rewind 완료 | 저장 장치 입출력 태스크 | `STORAGE_LOOP_REWIND_DONE` | 일반 queue | 트랙 파일 끝에 도달한 뒤 반복 재생을 위해 읽기 위치를 되돌렸음을 알린다. |

### 1.3 저장 장치 입출력 태스크 수신 메시지

| 메시지 종류 | 송신 태스크 | 메시지 타입 | 수신 방식 | 간단한 설명 |
| --- | --- | --- | --- | --- |
| 쓰기 파일 열기 | 루프스테이션 상태 관리 태스크 | `TRACK_FILE_OPEN_WRITE` | 일반 queue | 녹음 또는 오버더빙 결과를 저장할 트랙 파일을 연다. |
| 읽기 파일 열기 | 루프스테이션 상태 관리 태스크 | `TRACK_FILE_OPEN_READ` | 일반 queue | 반복 재생할 트랙 파일을 연다. |
| 파일 닫기 | 루프스테이션 상태 관리 태스크 | `TRACK_FILE_CLOSE` | 일반 queue | 트랙 파일을 닫고 필요하면 WAV header 크기 정보를 갱신한다. |
| 파일 삭제 | 루프스테이션 상태 관리 태스크 | `TRACK_FILE_DELETE` | 일반 queue | 정지 버튼 길게 누름 또는 연속 입력에 따라 트랙 파일 삭제를 요청한다. |
| 파일 위치 이동 | 루프스테이션 상태 관리 태스크 | `TRACK_FILE_SEEK` | 일반 queue | 반복 재생을 위해 파일 읽기 위치를 처음 또는 특정 frame으로 이동한다. |
| storage chunk 쓰기 요청 | 오디오 처리 태스크 | `STORAGE_WRITE_CHUNK_REQ` | 일반 queue | 녹음된 WAV PCM storage chunk를 SD 카드에 쓰도록 요청한다. |
| storage chunk 읽기 요청 | 오디오 처리 태스크 | `STORAGE_READ_CHUNK_REQ` | 일반 queue | 재생 또는 오버더빙에 필요한 트랙 데이터를 SD 카드에서 읽도록 요청한다. |

### 1.4 LED/디스플레이 처리 태스크 수신 메시지

| 메시지 종류 | 송신 태스크 | 메시지 타입 | 수신 방식 | 간단한 설명 |
| --- | --- | --- | --- | --- |
| 초기 화면 표시 | 루프스테이션 상태 관리 태스크 | `DISPLAY_INIT` | `display_command_queue` | 부팅 완료 후 LCD를 홈 패널로 초기화한다. |
| UI 화면 전환/상태 표시 | 루프스테이션 상태 관리 태스크 | `UI_STATE_RENDER` | `display_command_queue` | 현재 시스템 상태와 선택된 패널을 LCD에 표시하도록 요청한다. |
| 트랙 상태 표시 | 루프스테이션 상태 관리 태스크 | `TRACK_STATE_RENDER` | `display_render_mailbox` | 트랙 상태, 트랙 길이, 트랙 LED 표시를 갱신한다. |
| FX 상태 표시 | 루프스테이션 상태 관리 태스크 | `FX_STATE_RENDER` | `display_render_mailbox` | FX 상태 패널, IFX/TFX 설정 패널, FX LED 표시를 갱신한다. |
| 시스템 설정 표시 | 루프스테이션 상태 관리 태스크 | `SYSTEM_SETTING_RENDER` | `display_render_mailbox` | 시스템 설정 패널의 표시값을 갱신한다. |
| LCD 밝기 변경 | 루프스테이션 상태 관리 태스크 | `LCD_BRIGHTNESS_SET` | `display_command_queue` | LCD 밝기 설정값을 실제 표시 장치 출력에 반영한다. |
| 레벨 미터 표시 | 루프스테이션 상태 관리 태스크 | `LEVEL_METER_RENDER` | `display_render_mailbox` | 오디오 처리 태스크에서 받은 레벨 정보를 LCD에 표시한다. |
| 하드웨어 점검 표시 | 루프스테이션 상태 관리 태스크 | `DIAGNOSTIC_RENDER` | `display_render_mailbox` | 버튼, LED, 포텐셔미터, 로터리 엔코더 검사 패널을 표시한다. |

### 1.5 사용자 컨트롤 처리 태스크 수신 메시지

사용자 컨트롤 처리 태스크는 버튼, 엔코더, 포텐셔미터의 raw event를 루프스테이션 상태 관리 태스크에 전달한다. 현재 패널이나 선택 항목에 따라 이 입력이 어떤 상태 변경으로 해석되는지는 루프스테이션 상태 관리 태스크가 담당한다.

따라서 현재 설계에서 사용자 컨트롤 처리 태스크가 수신해야 하는 메시지는 정의하지 않는다.

### 1.6 전체 태스크 대상 메시지

| 메시지 종류 | 송신 태스크 | 메시지 타입 | 수신 방식 | 간단한 설명 |
| --- | --- | --- | --- | --- |
| 시스템 초기화 요청 | 루프스테이션 상태 관리 태스크 | `SYSTEM_RESET_REQUEST` | TODO | 시스템 초기화 요청에 따라 상태, 오디오, 저장, 표시 관련 작업을 초기 상태로 되돌리도록 요청한다. |
| 시스템 정지 요청 | 루프스테이션 상태 관리 태스크 | `SYSTEM_STOP` | TODO | 치명적 오류, 사용자 초기화, 전원 종료 대응 시 각 태스크가 진행 중 작업을 멈추고 정리하도록 요청한다. |

## 2. 후속 설계에서 결정할 내용

- `TODO`로 남긴 시스템 초기화/정지 요청과 오디오 DMA 이벤트의 수신 방식을 확정한다.
- `AUDIO_RX_BLOCK_READY`, `AUDIO_TX_BLOCK_READY`처럼 실시간성이 높은 이벤트는 후속 오디오 DMA 설계에서 별도로 확정한다.
- `STORAGE_WRITE_CHUNK_REQ`, `STORAGE_READ_CHUNK_READY`는 buffer pointer를 포함하므로 buffer pool과 소유권 이전 규칙을 함께 정의한다.
- LED/디스플레이 처리 태스크의 표시 갱신 메시지는 `display_render_mailbox` 사용을 기준으로 큐 길이와 overwrite 정책을 확정한다.
- 오류 메시지는 향후 [오류 처리 정책 문서](./error_handling_policy.md)에서 severity, 복구 가능 여부, 사용자 표시 정책과 연결한다.
- 트랙 파일 open/read/write/delete 정책은 향후 [트랙 파일 포맷/저장 정책 문서](../storage/track_file_storage_policy.md)에서 파일명, 디렉터리, metadata, split 정책과 함께 확정한다.
- 녹음 종료 후 재생 전환 지연, RAM prebuffer, read/write 파일 핸들 유지 여부는 [녹음 후 재생 전환 정책 TBA](../storage/record_to_playback_transition_tba.md)에서 별도로 검토한다.
