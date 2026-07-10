---
title: 오디오 처리 태스크 수신 메시지 스키마
version: 0.1.0
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: 오디오 처리 태스크가 수신하는 메시지 목록과 공통 스키마를 분리하고 트랙 loop 구간 변경 메시지를 제외함
---

# 오디오 처리 태스크 수신 메시지 스키마

이 문서는 오디오 처리 태스크가 수신하는 command, config, storage result, DMA event 메시지를 정의한다. 오디오 처리 태스크는 실시간 처리를 위해 필요한 값을 runtime snapshot으로 보관한다.

## 1. Queue

| Queue | 수신 방식 | 설명 |
| --- | --- | --- |
| `audio_command_queue` | 일반 queue | 상태 관리 태스크와 저장 장치 태스크가 보내는 command/result를 수신한다. |
| `audio_dma_event_queue` | TODO | SAI DMA half/full event처럼 실시간성이 높은 이벤트를 수신한다. |

## 2. 공통 envelope

`audio_command_queue`는 하나의 공통 envelope와 payload union을 사용한다.

```c
typedef struct {
    AudioMessageType type;
    TaskId source;
    uint32_t timestamp_ms;
    union {
        AudioTrackCommandPayload track_command;
        AudioConfigPayload config;
        AudioFxPayload fx;
        AudioTempoPayload tempo;
        TrackFileResultPayload file_result;
        StorageChunkResultPayload chunk_result;
        AudioDmaBlockPayload dma_block;
    } payload;
} AudioMessage;
```

## 3. 메시지 목록

| 메시지 종류 | 송신 태스크 | 메시지 타입 | Payload | 간단한 설명 |
| --- | --- | --- | --- | --- |
| 입력 패스스루 설정 | 루프스테이션 상태 관리 태스크 | `AUDIO_PASSTHROUGH_ENABLE` | `AudioConfigPayload` | IFX 이후 입력 신호를 믹싱 경로에 보낼지 설정한다. |
| 녹음 시작 | 루프스테이션 상태 관리 태스크 | `TRACK_RECORD_START` | `AudioTrackCommandPayload` | 첫 녹음을 시작한다. |
| 녹음 종료 요청 | 루프스테이션 상태 관리 태스크 | `TRACK_RECORD_FINISH_REQUEST` | `AudioTrackCommandPayload` | 종료 요청 시점을 기준으로 오디오 처리 태스크가 `target_end_frame`을 계산한다. |
| 녹음 정지 | 루프스테이션 상태 관리 태스크 | `TRACK_RECORD_STOP` | `AudioTrackCommandPayload` | 첫 녹음을 중단하고 정지한다. |
| 재생 시작 | 루프스테이션 상태 관리 태스크 | `TRACK_PLAY_START` | `AudioTrackCommandPayload` | 트랙 반복 재생을 시작한다. |
| 재생 정지 | 루프스테이션 상태 관리 태스크 | `TRACK_PLAY_STOP` | `AudioTrackCommandPayload` | 재생 중인 트랙을 정지한다. |
| 오버더빙 시작 | 루프스테이션 상태 관리 태스크 | `TRACK_OVERDUB_START` | `AudioTrackCommandPayload` | 현재 재생 위치에서 오버더빙 쓰기를 시작한다. |
| 오버더빙 완료 | 루프스테이션 상태 관리 태스크 | `TRACK_OVERDUB_FINISH` | `AudioTrackCommandPayload` | 오버더빙 쓰기를 마무리하고 로컬 실행 상태를 재생으로 전환한다. |
| 오버더빙 정지 | 루프스테이션 상태 관리 태스크 | `TRACK_OVERDUB_STOP` | `AudioTrackCommandPayload` | 오버더빙 쓰기를 마무리하고 로컬 실행 상태를 정지로 전환한다. |
| FX 활성화 설정 | 루프스테이션 상태 관리 태스크 | `FX_ENABLE_SET` | `AudioFxPayload` | IFX 또는 TFX 활성화 상태를 반영한다. |
| FX 선택 | 루프스테이션 상태 관리 태스크 | `FX_SELECT` | `AudioFxPayload` | IFX/TFX 종류를 변경한다. |
| FX 파라미터 변경 | 루프스테이션 상태 관리 태스크 | `FX_PARAM_SET` | `AudioFxPayload` | FX 파라미터를 변경한다. |
| 트랙 gain 변경 | 루프스테이션 상태 관리 태스크 | `TRACK_GAIN_SET` | `AudioConfigPayload` | 트랙별 gain 값을 반영한다. |
| 트랙 TFX 설정 | 루프스테이션 상태 관리 태스크 | `TRACK_TFX_SET` | `AudioConfigPayload` | 트랙별 TFX 적용 여부를 반영한다. |
| 트랙 재생 방향 변경 | 루프스테이션 상태 관리 태스크 | `TRACK_PLAY_DIRECTION_SET` | `AudioConfigPayload` | 트랙 재생 방향을 설정한다. |
| BPM 변경 | 루프스테이션 상태 관리 태스크 | `TEMPO_SET` | `AudioTempoPayload` | canonical system BPM을 runtime snapshot에 반영한다. |
| master gain 변경 | 루프스테이션 상태 관리 태스크 | `MASTER_GAIN_SET` | `AudioConfigPayload` | 최종 출력 gain 값을 반영한다. |
| 오디오 입력 block 준비 | SAI DMA callback | `AUDIO_RX_BLOCK_READY` | `AudioDmaBlockPayload` | SAI RX half/full block 준비를 알린다. |
| 파일 열기 완료 | 저장 장치 입출력 태스크 | `TRACK_FILE_OPEN_DONE` | `TrackFileResultPayload` | 트랙 파일 open 성공 결과를 전달한다. |
| 파일 열기 실패 | 저장 장치 입출력 태스크 | `TRACK_FILE_OPEN_FAILED` | `TrackFileResultPayload` | 트랙 파일 open 실패 결과를 전달한다. |
| 파일 닫기 완료 | 저장 장치 입출력 태스크 | `TRACK_FILE_CLOSE_DONE` | `TrackFileResultPayload` | 트랙 파일 close 및 WAV header 갱신 완료를 전달한다. |
| 파일 닫기 실패 | 저장 장치 입출력 태스크 | `TRACK_FILE_CLOSE_FAILED` | `TrackFileResultPayload` | 트랙 파일 close 실패를 전달한다. |
| 파일 위치 이동 완료 | 저장 장치 입출력 태스크 | `TRACK_FILE_SEEK_DONE` | `TrackFileResultPayload` | 파일 위치 이동 성공을 전달한다. |
| 파일 위치 이동 실패 | 저장 장치 입출력 태스크 | `TRACK_FILE_SEEK_FAILED` | `TrackFileResultPayload` | 파일 위치 이동 실패를 전달한다. |
| storage chunk 쓰기 완료 | 저장 장치 입출력 태스크 | `STORAGE_WRITE_CHUNK_DONE` | `StorageChunkResultPayload` | chunk write 완료와 버퍼 반환 가능 여부를 전달한다. |
| storage chunk 읽기 완료 | 저장 장치 입출력 태스크 | `STORAGE_READ_CHUNK_READY` | `StorageChunkResultPayload` | read chunk 준비를 전달한다. |
| storage chunk 처리 실패 | 저장 장치 입출력 태스크 | `STORAGE_CHUNK_IO_FAILED` | `StorageChunkResultPayload` | chunk read/write 실패를 전달한다. |
| loop rewind 완료 | 저장 장치 입출력 태스크 | `STORAGE_LOOP_REWIND_DONE` | `TrackFileResultPayload` | 반복 재생 read pointer 되감기 완료를 전달한다. |

## 4. Payload 스키마

### 4.1 `AudioTrackCommandPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `command` | `TrackCommand` | record start/finish request/stop, play start/stop, overdub start/finish/stop |
| `request_frame` | `uint32_t` | 명령 수신 기준 frame. 필요 없으면 `0` |
| `record_start_frame` | `uint32_t` | 첫 녹음 시작 frame. finish request 계산에 사용 |
| `bpm_x100` | `uint32_t` | BPM snapshot. 120.00 BPM은 `12000` |
| `beats_per_measure` | `uint8_t` | 박자표 분자 |
| `candidate_mask` | `uint32_t` | 허용 후보 길이 bitmask. 1/8, 1/4, 1/2, 1, 2, 3, n마디 등 |

`TRACK_RECORD_FINISH_REQUEST`는 `target_end_frame`을 직접 전달하지 않는다. 오디오 처리 태스크가 `request_frame`, `record_start_frame`, `bpm_x100`, `beats_per_measure`, `candidate_mask`로 계산한다.

### 4.2 `AudioConfigPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `target` | `AudioConfigTarget` | passthrough, track gain, track TFX, play direction, master gain 등 |
| `track_id` | `uint8_t` | 대상 트랙. 전체 설정이면 `TRACK_ID_NONE` |
| `value_i32` | `int32_t` | 정수 설정값 |
| `value_u32` | `uint32_t` | unsigned 설정값 또는 bitmask |

단순 enable/gain/direction 계열 설정은 이 공통 payload를 우선 사용한다.

### 4.3 `AudioFxPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `fx_bus` | `FxBus` | IFX 또는 TFX |
| `track_id` | `uint8_t` | TFX 대상 트랙. IFX 또는 global이면 `TRACK_ID_NONE` |
| `fx_id` | `FxId` | FX 종류 |
| `param_id` | `FxParamId` | 파라미터 ID. enable/select에는 사용하지 않을 수 있다. |
| `value` | `int32_t` | 파라미터 값 |
| `enabled` | `bool` | 활성화 여부 |

### 4.4 `AudioTempoPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `bpm_x100` | `uint32_t` | canonical system BPM |
| `beats_per_measure` | `uint8_t` | 박자표 분자 |
| `beat_unit` | `uint8_t` | 박자표 분모 |
| `tempo_sync_enabled_mask` | `uint32_t` | tempo sync 적용 트랙 bitmask |

### 4.5 `TrackFileResultPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `request_id` | `uint32_t` | 오디오 처리 태스크가 보낸 요청 ID |
| `track_id` | `uint8_t` | 대상 트랙 |
| `result` | `ResultCode` | 성공 또는 실패 |
| `file_handle_id` | `uint16_t` | 저장 장치 태스크가 관리하는 파일 핸들 ID |
| `data_start_offset` | `uint32_t` | WAV data chunk 시작 offset |
| `total_frames` | `uint32_t` | 읽기 open 시 확인한 전체 frame 수 |
| `error_code` | `int32_t` | 실패 원인. 성공이면 `0` |

### 4.6 `StorageChunkResultPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `request_id` | `uint32_t` | 오디오 처리 태스크가 보낸 요청 ID |
| `track_id` | `uint8_t` | 대상 트랙 |
| `buffer_id` | `uint16_t` | buffer pool ID |
| `buffer_ptr` | `void *` | 오디오 buffer pointer. 실제 구현에서는 타입 확정 필요 |
| `frame_count` | `uint32_t` | chunk에 포함된 frame 수 |
| `file_frame_offset` | `uint32_t` | 파일 기준 frame offset |
| `result` | `ResultCode` | 성공 또는 실패 |
| `error_code` | `int32_t` | 실패 원인. 성공이면 `0` |

### 4.7 `AudioDmaBlockPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `buffer_id` | `uint16_t` | DMA buffer ID |
| `half` | `DmaHalf` | half 또는 full |
| `frame_count` | `uint32_t` | 준비된 frame 수 |
| `timestamp_frame` | `uint32_t` | 오디오 frame clock 기준 시각 |
