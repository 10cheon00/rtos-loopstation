---
title: 루프스테이션 상태 관리 태스크 수신 메시지 스키마
version: 0.1.0
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: 루프스테이션 상태 관리 태스크가 수신하는 메시지 목록과 스키마를 분리해 정리함
---

# 루프스테이션 상태 관리 태스크 수신 메시지 스키마

이 문서는 루프스테이션 상태 관리 태스크가 수신하는 메시지와 payload 스키마를 정의한다. 상태 관리 태스크는 사용자 입력과 시스템 이벤트를 해석해 canonical state를 갱신하고, 필요한 실행 태스크에 command/config 메시지를 전파한다.

## 1. Queue

| Queue | 수신 방식 | 설명 |
| --- | --- | --- |
| `state_event_queue` | 일반 queue | 순서가 중요하고 누락되면 안 되는 control/system event를 수신한다. |

## 2. 공통 envelope

`state_event_queue`에 들어가는 메시지는 하나의 공통 envelope를 사용한다.

```c
typedef struct {
    StateEventType type;
    TaskId source;
    uint32_t timestamp_ms;
    union {
        ControlButtonPayload button;
        ControlEncoderPayload encoder;
        ControlPotPayload pot;
        AudioRecordDonePayload record_done;
        AudioErrorPayload audio_error;
        TrackFileResetResultPayload track_reset_result;
        StorageMediaStatusPayload media_status;
        StorageErrorPayload storage_error;
        SystemEventPayload system;
    } payload;
} StateEventMessage;
```

## 3. 메시지 목록

| 메시지 종류 | 송신 태스크 | 메시지 타입 | Payload | 간단한 설명 |
| --- | --- | --- | --- | --- |
| 시스템 부팅 | 시스템 초기화 흐름 | `SYSTEM_BOOT` | `SystemEventPayload` | 전원 인가 후 상태 모델을 초기화하고 초기 화면 표시를 준비한다. |
| 버튼 입력 | 사용자 컨트롤 처리 태스크 | `CONTROL_BUTTON` | `ControlButtonPayload` | 모든 버튼과 엔코더 push의 press/release 상태를 전달한다. |
| 엔코더 회전 | 사용자 컨트롤 처리 태스크 | `CONTROL_ENCODER_ROTATE` | `ControlEncoderPayload` | 회전 방향, 이동량, modifier snapshot을 전달한다. |
| 포텐셔미터 값 변경 | 사용자 컨트롤 처리 태스크 | `CONTROL_POT_CHANGE` | `ControlPotPayload` | threshold/rate limit을 거친 ADC 포텐셔미터 변경값을 전달한다. |
| 녹음 완료 보고 | 오디오 처리 태스크 | `AUDIO_RECORD_DONE` | `AudioRecordDonePayload` | 첫 녹음이 실제 종료되고 파일 close까지 완료되었음을 보고한다. |
| 오디오 트랙 I/O 실패 보고 | 오디오 처리 태스크 | `AUDIO_TRACK_IO_FAILED` | `AudioErrorPayload` | 오디오 처리에 필요한 트랙 파일 I/O 실패를 보고한다. |
| 오디오 dropout 보고 | 오디오 처리 태스크 | `AUDIO_DROPOUT_DETECTED` | `AudioErrorPayload` | 오디오 block 처리 지연 또는 누락을 보고한다. |
| 트랙 파일 초기화 완료 | 저장 장치 입출력 태스크 | `TRACK_FILE_RESET_DONE` | `TrackFileResetResultPayload` | 트랙 파일은 유지한 채 음원 데이터 초기화가 완료되었음을 전달한다. |
| 트랙 파일 초기화 실패 | 저장 장치 입출력 태스크 | `TRACK_FILE_RESET_FAILED` | `TrackFileResetResultPayload` | 트랙 파일 음원 데이터 초기화 실패를 전달한다. |
| 저장장치 상태 | 저장 장치 입출력 태스크 | `STORAGE_MEDIA_STATUS` | `StorageMediaStatusPayload` | SD 카드 삽입, mount 상태, 남은 용량을 보고한다. |
| 저장장치 오류 | 저장 장치 입출력 태스크 | `STORAGE_ERROR` | `StorageErrorPayload` | SD 카드, FatFs, 파일 처리 오류를 보고한다. |

## 4. Payload 스키마

### 4.1 `ControlButtonPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `button_id` | `ButtonId` | record/play, stop, ifx, tfx, left, right, enter, exit, encoder push 등 물리 버튼 ID |
| `state` | `ButtonState` | `PRESSED` 또는 `RELEASED` |
| `timestamp_ms` | `uint32_t` | 사용자 컨트롤 처리 태스크가 감지한 이벤트 시각 |

길게 누름, 반복 입력, double click은 별도 메시지로 보내지 않는다. 상태 관리 태스크가 press/release 시각을 기준으로 해석한다.

### 4.2 `ControlEncoderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `encoder_id` | `EncoderId` | 로터리 엔코더 ID |
| `delta` | `int16_t` | 회전량. 시계 방향은 양수, 반시계 방향은 음수 |
| `step_count` | `uint16_t` | 누적 step 수 또는 이벤트 단위 step 수 |
| `modifier_mask` | `uint32_t` | 회전 순간 눌려 있던 버튼 modifier snapshot |
| `timestamp_ms` | `uint32_t` | 회전 이벤트 시각 |

### 4.3 `ControlPotPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `pot_id` | `PotId` | 포텐셔미터 ID |
| `raw_value` | `uint16_t` | ADC raw 값 |
| `normalized_value` | `uint16_t` | UI/파라미터 처리용 정규화 값. 범위는 후속 구현에서 확정한다. |
| `timestamp_ms` | `uint32_t` | 변경 감지 시각 |

### 4.4 `AudioRecordDonePayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 녹음이 완료된 트랙 |
| `record_start_frame` | `uint32_t` | 첫 녹음 시작 frame |
| `target_end_frame` | `uint32_t` | 오디오 처리 태스크가 계산한 실제 녹음 종료 frame |
| `recorded_frames` | `uint32_t` | 확정된 녹음 길이 |
| `original_bpm_x100` | `uint32_t` | 녹음 시작 시점의 BPM snapshot. 120.00 BPM은 `12000` |
| `measure_length_x1000` | `uint32_t` | 선택된 후보 마디 길이. 0.5마디는 `500` |

상태 관리 태스크는 이 메시지의 payload로 다음 상태를 결정하지 않는다. `TRACK_RECORD_FINISH_REQUEST`를 보낼 때 로컬에 저장해 둔 목표 상태로 전이한다.

### 4.5 `AudioErrorPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 관련 트랙. 전체 오디오 오류면 `TRACK_ID_NONE` |
| `operation` | `AudioOperation` | record, play, overdub, file open, file close, chunk read/write 등 |
| `error_code` | `int32_t` | 내부 오류 코드 또는 FatFs/HAL 오류 코드 |
| `frame_index` | `uint32_t` | 오류가 발생한 오디오 frame. 알 수 없으면 `0` |

### 4.6 `TrackFileResetResultPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 초기화 대상 트랙 |
| `result` | `ResultCode` | 성공 또는 실패 |
| `error_code` | `int32_t` | 실패 원인. 성공이면 `0` |

### 4.7 `StorageMediaStatusPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `inserted` | `bool` | SD 카드 삽입 여부 |
| `mounted` | `bool` | FatFs mount 여부 |
| `free_bytes` | `uint64_t` | 남은 용량 |
| `total_bytes` | `uint64_t` | 전체 용량 |

### 4.8 `StorageErrorPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `operation` | `StorageOperation` | mount, open, read, write, close, reset 등 |
| `track_id` | `uint8_t` | 관련 트랙. 없으면 `TRACK_ID_NONE` |
| `error_code` | `int32_t` | FatFs 또는 storage driver 오류 코드 |

### 4.9 `SystemEventPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `reason` | `SystemEventReason` | boot, reset, stop 등 시스템 이벤트 원인 |
