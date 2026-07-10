---
title: 저장 장치 입출력 태스크 수신 메시지 스키마
version: 0.3.0
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: 저장 장치 입출력 태스크가 수신하는 파일 및 chunk 요청 메시지 스키마를 정리함
  - date: 2026-07-10
    version: 0.2.0
    summary: 트랙 파일 열기 요청을 read-write 모드 단일 메시지로 통합함
  - date: 2026-07-10
    version: 0.3.0
    summary: 파일 포인터 이동 전용 요청 메시지를 제거하고 chunk offset 기반 I/O로 정리함
---

# 저장 장치 입출력 태스크 수신 메시지 스키마

이 문서는 저장 장치 입출력 태스크가 수신하는 요청 메시지를 정의한다. 저장 장치 태스크는 파일 시스템 작업을 수행하고, 결과를 요청한 태스크로 되돌려준다.

## 1. Queue

| Queue | 수신 방식 | 설명 |
| --- | --- | --- |
| `storage_request_queue` | 일반 queue | 파일 open/read/write/close/reset 요청과 chunk read/write 요청을 수신한다. 트랙 오디오 파일 open은 read-write 모드를 기본으로 한다. |

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
        StorageChunkRequestPayload chunk;
    } payload;
} StorageRequestMessage;
```

## 3. 메시지 목록

| 메시지 종류 | 송신 태스크 | 메시지 타입 | Payload | 간단한 설명 |
| --- | --- | --- | --- | --- |
| 트랙 파일 열기 | 오디오 처리 태스크 | `TRACK_FILE_OPEN_RW` | `TrackFileOpenPayload` | 녹음, 재생, 오버더빙에 사용할 트랙 오디오 파일을 read-write 모드로 연다. |
| 파일 닫기 | 오디오 처리 태스크 | `TRACK_FILE_CLOSE` | `TrackFileClosePayload` | 트랙 파일을 닫고 필요하면 WAV header 정보를 갱신한다. |
| 트랙 파일 초기화 | 루프스테이션 상태 관리 태스크 | `TRACK_FILE_RESET` | `TrackFileResetPayload` | 트랙 파일은 유지하고 내부 음원 데이터를 초기화한다. |
| storage chunk 쓰기 요청 | 오디오 처리 태스크 | `STORAGE_WRITE_CHUNK_REQ` | `StorageChunkRequestPayload` | PCM storage chunk를 SD 카드에 쓴다. |
| storage chunk 읽기 요청 | 오디오 처리 태스크 | `STORAGE_READ_CHUNK_REQ` | `StorageChunkRequestPayload` | 재생 또는 오버더빙에 필요한 트랙 데이터를 읽는다. |

## 4. Payload 스키마

### 4.1 `TrackFileOpenPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `create_if_missing` | `bool` | 파일이 없으면 생성할지 여부 |
| `truncate_audio_data` | `bool` | 기존 파일은 유지하되 audio data 영역을 초기화할지 여부 |

트랙 오디오 파일은 항상 read-write 모드로 연다. FatFs 기준으로 `FA_READ | FA_WRITE`는 open 시 접근 권한과 읽기 전용 속성 위반 여부를 확인하는 의미가 크며, 이후 chunk read/write의 주된 비용은 SD 카드 접근, seek, cluster allocation, dirty sector write-back에서 발생한다. 따라서 read 전용/쓰기 전용 메시지를 나누기보다 단일 read-write open 요청으로 통합한다.

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

### 4.4 `StorageChunkRequestPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `file_handle_id` | `uint16_t` | read/write 대상 파일 핸들 |
| `buffer_id` | `uint16_t` | buffer pool ID |
| `buffer_ptr` | `void *` | read/write buffer pointer. 실제 타입은 구현 시 확정 |
| `frame_count` | `uint32_t` | 처리할 frame 수 |
| `file_frame_offset` | `uint32_t` | 파일 기준 frame offset |
| `direction` | `StorageChunkDirection` | read 또는 write |

오디오 처리 태스크는 파일 포인터 이동을 별도 명령으로 요청하지 않는다. 반복 재생에서 loop 시작점으로 돌아가야 하거나, 오버더빙에서 같은 offset에 다시 써야 하는 경우에도 다음 chunk 요청의 `file_frame_offset`에 대상 위치를 담는다. 저장 장치 입출력 태스크는 이 offset을 기준으로 내부에서 필요한 `f_lseek()` 여부를 판단한다.
