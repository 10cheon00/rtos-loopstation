---
title: 저장 장치 입출력 태스크 수신 메시지 스키마
version: 0.1.0
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: 저장 장치 입출력 태스크가 수신하는 파일 및 chunk 요청 메시지 스키마를 정리함
---

# 저장 장치 입출력 태스크 수신 메시지 스키마

이 문서는 저장 장치 입출력 태스크가 수신하는 요청 메시지를 정의한다. 저장 장치 태스크는 파일 시스템 작업을 수행하고, 결과를 요청한 태스크로 되돌려준다.

## 1. Queue

| Queue | 수신 방식 | 설명 |
| --- | --- | --- |
| `storage_request_queue` | 일반 queue | 파일 open/read/write/close/reset/seek 요청을 수신한다. |

## 2. 공통 envelope

```c
typedef struct {
    StorageRequestType type;
    TaskId requester;
    uint32_t request_id;
    uint32_t timestamp_ms;
    union {
        TrackFileOpenPayload open;
        TrackFileClosePayload close;
        TrackFileResetPayload reset;
        TrackFileSeekPayload seek;
        StorageChunkRequestPayload chunk;
        StorageLoopRewindPayload rewind;
    } payload;
} StorageRequestMessage;
```

## 3. 메시지 목록

| 메시지 종류 | 송신 태스크 | 메시지 타입 | Payload | 간단한 설명 |
| --- | --- | --- | --- | --- |
| 쓰기 파일 열기 | 오디오 처리 태스크 | `TRACK_FILE_OPEN_WRITE` | `TrackFileOpenPayload` | 녹음 또는 오버더빙 결과를 저장할 트랙 파일을 연다. |
| 읽기 파일 열기 | 오디오 처리 태스크 | `TRACK_FILE_OPEN_READ` | `TrackFileOpenPayload` | 반복 재생할 트랙 파일을 연다. |
| 파일 닫기 | 오디오 처리 태스크 | `TRACK_FILE_CLOSE` | `TrackFileClosePayload` | 트랙 파일을 닫고 필요하면 WAV header 정보를 갱신한다. |
| 트랙 파일 초기화 | 루프스테이션 상태 관리 태스크 | `TRACK_FILE_RESET` | `TrackFileResetPayload` | 트랙 파일은 유지하고 내부 음원 데이터를 초기화한다. |
| 파일 위치 이동 | 오디오 처리 태스크 | `TRACK_FILE_SEEK` | `TrackFileSeekPayload` | 파일 읽기 위치를 특정 frame으로 이동한다. |
| storage chunk 쓰기 요청 | 오디오 처리 태스크 | `STORAGE_WRITE_CHUNK_REQ` | `StorageChunkRequestPayload` | PCM storage chunk를 SD 카드에 쓴다. |
| storage chunk 읽기 요청 | 오디오 처리 태스크 | `STORAGE_READ_CHUNK_REQ` | `StorageChunkRequestPayload` | 재생 또는 오버더빙에 필요한 트랙 데이터를 읽는다. |
| loop rewind 요청 | 오디오 처리 태스크 | `STORAGE_LOOP_REWIND_REQ` | `StorageLoopRewindPayload` | 반복 재생 중 read pointer를 loop 시작점으로 되돌린다. |

## 4. Payload 스키마

### 4.1 `TrackFileOpenPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `mode` | `TrackFileOpenMode` | read 또는 write |
| `create_if_missing` | `bool` | 파일이 없으면 생성할지 여부 |
| `truncate_audio_data` | `bool` | 기존 파일은 유지하되 audio data 영역을 초기화할지 여부 |

### 4.2 `TrackFileClosePayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `file_handle_id` | `uint16_t` | 닫을 파일 핸들 |
| `final_frame_count` | `uint32_t` | WAV header에 반영할 최종 frame 수 |
| `update_header` | `bool` | WAV header를 갱신할지 여부 |

### 4.3 `TrackFileResetPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 초기화 대상 트랙 |
| `reset_audio_data` | `bool` | audio data 영역 초기화 여부 |
| `reset_metadata` | `bool` | metadata 초기화 여부 |

파일 자체를 삭제하지 않는다.

### 4.4 `TrackFileSeekPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `file_handle_id` | `uint16_t` | seek 대상 파일 핸들 |
| `target_frame` | `uint32_t` | 이동할 frame 위치 |

### 4.5 `StorageChunkRequestPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `file_handle_id` | `uint16_t` | read/write 대상 파일 핸들 |
| `buffer_id` | `uint16_t` | buffer pool ID |
| `buffer_ptr` | `void *` | read/write buffer pointer. 실제 타입은 구현 시 확정 |
| `frame_count` | `uint32_t` | 처리할 frame 수 |
| `file_frame_offset` | `uint32_t` | 파일 기준 frame offset |
| `direction` | `StorageChunkDirection` | read 또는 write |

### 4.6 `StorageLoopRewindPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `file_handle_id` | `uint16_t` | rewind 대상 파일 핸들 |
| `loop_start_frame` | `uint32_t` | 되돌릴 loop 시작 frame |
