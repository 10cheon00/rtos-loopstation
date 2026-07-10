---
title: 저장 장치 입출력 메시지 시퀀스
version: 0.4.0
change_history:
  - date: 2026-07-10
    version: 0.1.0
    summary: 저장 장치 입출력 태스크의 파일 및 chunk 처리 메시지 흐름을 시퀀스 다이어그램으로 정리함
  - date: 2026-07-10
    version: 0.2.0
    summary: 트랙 상태 표시 메시지를 전체 snapshot 전달 방식으로 갱신함
  - date: 2026-07-10
    version: 0.3.0
    summary: 트랙 파일 open 정책을 read-write 모드 단일 handle 기준으로 확정함
  - date: 2026-07-10
    version: 0.3.1
    summary: 오버더빙 저장 방식이 현재는 destructive overwrite 전제임을 명시함
  - date: 2026-07-10
    version: 0.4.0
    summary: 파일 포인터 이동 전용 메시지를 제거하고 chunk offset 기반 흐름으로 정리함
---

# 저장 장치 입출력 메시지 시퀀스

이 문서는 저장 장치 입출력 태스크가 어떤 요청을 받고, 처리 결과를 어느 태스크로 되돌려주는지 시퀀스 다이어그램으로 정리한다.

참고 문서:

| 문서 | 참고 내용 |
| --- | --- |
| [storage_task_messages.md](./messages/storage_task_messages.md) | 저장 장치 입출력 태스크 수신 요청 메시지와 payload |
| [audio_task_messages.md](./messages/audio_task_messages.md) | 오디오 처리 태스크가 수신하는 저장 장치 처리 결과 메시지 |
| [state_task_messages.md](./messages/state_task_messages.md) | 상태 관리 태스크가 수신하는 트랙 초기화 결과와 저장 장치 상태 메시지 |
| [recording_flow.md](../audio/recording_flow.md) | 첫 녹음과 오버더빙 중 저장 장치 요청 흐름 |
| TODO: 트랙 파일 포맷/저장 정책 문서 | 트랙 파일명, 디렉터리, metadata, split 정책 |

## 1. 기본 원칙

저장 장치 입출력 태스크는 `storage_request_queue`로 파일과 chunk 처리 요청을 받는다. 요청 메시지에는 `requester`와 `request_id`가 포함되며, 저장 장치 입출력 태스크는 처리 결과를 요청한 태스크로 되돌려준다.

오디오 처리 중 필요한 파일 open/read/write/close는 오디오 처리 태스크가 직접 저장 장치 입출력 태스크에 요청한다. 이 결과는 루프스테이션 상태 관리 태스크를 거치지 않고 오디오 처리 태스크로 직접 돌아간다.

상태 관리 태스크가 직접 저장 장치 입출력 태스크에 요청하는 경우는 현재 트랙 파일 초기화가 대표적이다. 이 경우 결과는 상태 관리 태스크가 받아 상태 모델과 표시 상태를 갱신한다.

트랙 오디오 파일은 녹음, 재생, 오버더빙 여부와 관계없이 `FA_READ | FA_WRITE`에 해당하는 read-write 모드로 연다. FatFs에서 read-write 모드는 open 시 접근 권한과 읽기 전용 속성 위반 여부를 확인하는 성격이 크며, 이후 성능은 open mode보다 SD 카드 접근, seek, cluster allocation, dirty sector write-back, chunk 크기에 더 크게 좌우된다.

단, read-write 모드로 연 하나의 `FIL` handle은 read pointer와 write pointer를 따로 갖지 않는다. 같은 handle로 읽기와 쓰기를 모두 수행할 때는 `STORAGE_READ_CHUNK_REQ`, `STORAGE_WRITE_CHUNK_REQ`의 `file_frame_offset`을 기준으로 저장 장치 입출력 태스크가 필요한 위치를 명확히 맞춘 뒤 I/O를 수행한다. 오디오 처리 태스크는 파일 포인터 이동을 별도 메시지로 요청하지 않는다.

## 2. 관련 Queue

| Queue | 송신 | 수신 | 용도 |
| --- | --- | --- | --- |
| `storage_request_queue` | 오디오 처리 태스크, 루프스테이션 상태 관리 태스크 | 저장 장치 입출력 태스크 | 파일 open/close/reset 요청과 chunk read/write 요청 |
| `audio_command_queue` | 저장 장치 입출력 태스크 | 오디오 처리 태스크 | 파일 처리 결과와 chunk 처리 결과 반환 |
| `state_event_queue` | 저장 장치 입출력 태스크 | 루프스테이션 상태 관리 태스크 | 트랙 초기화 결과, 저장 장치 상태, 저장 장치 오류 보고 |

## 3. 첫 녹음 파일 쓰기 흐름

첫 녹음에서는 오디오 처리 태스크가 트랙 파일을 read-write 모드로 열고, 녹음 중 chunk 쓰기를 반복한 뒤, 실제 녹음 종료 시 파일을 닫는다. 파일 close 결과까지 받은 뒤 오디오 처리 태스크가 상태 관리 태스크에 `AUDIO_RECORD_DONE`을 보고한다.

```mermaid
sequenceDiagram
    participant State as 루프스테이션 상태 관리 태스크
    participant Audio as 오디오 처리 태스크
    participant Storage as 저장 장치 입출력 태스크

    State->>Audio: TRACK_RECORD_START
    Audio->>Storage: TRACK_FILE_OPEN_RW(request_id, track_id, create_if_missing=true, truncate_audio_data=true)
    alt open 성공
        Storage-->>Audio: TRACK_FILE_OPEN_DONE(request_id, file_handle_id)
    else open 실패
        Storage-->>Audio: TRACK_FILE_OPEN_FAILED(request_id, error_code)
        Audio-->>State: AUDIO_TRACK_IO_FAILED(operation=open)
    end

    loop 녹음 chunk 쓰기
        Audio->>Storage: STORAGE_WRITE_CHUNK_REQ(request_id, file_handle_id, buffer_id, frame_count)
        alt write 성공
            Storage-->>Audio: STORAGE_WRITE_CHUNK_DONE(request_id, buffer_id)
            Audio->>Audio: buffer 반환 가능 처리
        else write 실패
            Storage-->>Audio: STORAGE_CHUNK_IO_FAILED(request_id, error_code)
            Audio-->>State: AUDIO_TRACK_IO_FAILED(operation=chunk_write)
        end
    end

    State->>Audio: TRACK_RECORD_FINISH_REQUEST
    Audio->>Audio: target_end_frame까지 녹음 지속
    Audio->>Storage: TRACK_FILE_CLOSE(request_id, final_frame_count, update_header=true)
    alt close 성공
        Storage-->>Audio: TRACK_FILE_CLOSE_DONE(request_id)
        Audio-->>State: AUDIO_RECORD_DONE(track_id, recorded_frames)
    else close 실패
        Storage-->>Audio: TRACK_FILE_CLOSE_FAILED(request_id, error_code)
        Audio-->>State: AUDIO_TRACK_IO_FAILED(operation=close)
    end
```

## 4. 반복 재생 파일 읽기 흐름

재생 시작 시 오디오 처리 태스크는 트랙 파일을 read-write 모드로 열고, 재생에 필요한 read chunk를 반복해서 요청한다. 파일 끝에 도달하면 loop 시작점으로 되감기 요청을 보낸다.

```mermaid
sequenceDiagram
    participant State as 루프스테이션 상태 관리 태스크
    participant Audio as 오디오 처리 태스크
    participant Storage as 저장 장치 입출력 태스크
    participant Display as LED/디스플레이 태스크

    State->>Audio: TRACK_PLAY_START
    Audio->>Storage: TRACK_FILE_OPEN_RW(request_id, track_id, create_if_missing=false, truncate_audio_data=false)
    alt open 성공
        Storage-->>Audio: TRACK_FILE_OPEN_DONE(request_id, file_handle_id, total_frames)
    else open 실패
        Storage-->>Audio: TRACK_FILE_OPEN_FAILED(request_id, error_code)
        Audio-->>State: AUDIO_TRACK_IO_FAILED(operation=open)
    end

    loop 반복 재생 중
        Audio->>Storage: STORAGE_READ_CHUNK_REQ(request_id, file_handle_id, file_frame_offset)
        alt read 성공
            Storage-->>Audio: STORAGE_READ_CHUNK_READY(request_id, buffer_id, frame_count)
            Audio->>Audio: 읽은 chunk를 재생 buffer로 연결
            Audio->>Display: AUDIO_TRACK_POSITION(track_id, position_frame)
        else read 실패
            Storage-->>Audio: STORAGE_CHUNK_IO_FAILED(request_id, error_code)
            Audio-->>State: AUDIO_TRACK_IO_FAILED(operation=chunk_read)
        end

        opt loop 끝 도달
            Audio->>Audio: 다음 read chunk의 file_frame_offset을 loop_start_frame으로 계산
        end
    end

    State->>Audio: TRACK_PLAY_STOP
    Audio->>Storage: TRACK_FILE_CLOSE(request_id, update_header=false)
    Storage-->>Audio: TRACK_FILE_CLOSE_DONE 또는 TRACK_FILE_CLOSE_FAILED
```

## 5. 오버더빙 파일 읽기/쓰기 흐름

오버더빙은 기존 트랙을 읽으면서 입력 오디오를 합친 결과를 다시 저장한다. 따라서 같은 오버더빙 구간에서 read 요청과 write 요청이 모두 발생한다.

```mermaid
sequenceDiagram
    participant State as 루프스테이션 상태 관리 태스크
    participant Audio as 오디오 처리 태스크
    participant Storage as 저장 장치 입출력 태스크
    participant Display as LED/디스플레이 태스크

    State->>Audio: TRACK_OVERDUB_START
    Audio->>Storage: TRACK_FILE_OPEN_RW(request_id, track_id, create_if_missing=false, truncate_audio_data=false)
    Storage-->>Audio: TRACK_FILE_OPEN_DONE 또는 TRACK_FILE_OPEN_FAILED

    loop 오버더빙 중
        Audio->>Storage: STORAGE_READ_CHUNK_REQ(read_request_id, file_handle_id, file_frame_offset)
        Storage-->>Audio: STORAGE_READ_CHUNK_READY 또는 STORAGE_CHUNK_IO_FAILED
        Audio->>Audio: 기존 트랙 chunk와 입력 오디오를 합성
        Audio->>Storage: STORAGE_WRITE_CHUNK_REQ(write_request_id, file_handle_id, buffer_id, file_frame_offset)
        Storage-->>Audio: STORAGE_WRITE_CHUNK_DONE 또는 STORAGE_CHUNK_IO_FAILED
        Audio->>Display: AUDIO_TRACK_POSITION(track_id, position_frame)
    end

    alt 오버더빙 후 재생
        State->>Audio: TRACK_OVERDUB_FINISH
        Audio->>Storage: TRACK_FILE_CLOSE(request_id, update_header=true)
        Storage-->>Audio: TRACK_FILE_CLOSE_DONE 또는 TRACK_FILE_CLOSE_FAILED
        Audio->>Audio: 로컬 실행 상태를 PLAYING으로 전환
    else 오버더빙 후 정지
        State->>Audio: TRACK_OVERDUB_STOP
        Audio->>Storage: TRACK_FILE_CLOSE(request_id, update_header=true)
        Storage-->>Audio: TRACK_FILE_CLOSE_DONE 또는 TRACK_FILE_CLOSE_FAILED
        Audio->>Audio: 로컬 실행 상태를 STOPPED로 전환
    end
```

오버더빙은 read와 write가 동시에 필요한 기능이지만, 실제 저장 장치 입출력은 `storage_request_queue`에서 순차 처리한다. 따라서 read handle과 write handle을 분리하지 않고 read-write handle 하나를 유지한다.

현재 시퀀스는 undo/redo가 없는 destructive overwrite를 전제로 한다. 기존 chunk를 읽고 입력 오디오와 합성한 뒤 같은 파일 offset에 다시 쓰므로, 향후 undo/redo를 추가하려면 기존 데이터를 즉시 덮어쓰지 않는 take 파일, delta 파일, snapshot 파일 정책을 별도로 설계해야 한다. 이 내용은 [record_to_playback_transition_tba.md](../storage/record_to_playback_transition_tba.md)의 undo/redo 확장 계획에서 추적한다.

## 6. 트랙 파일 초기화 흐름

트랙 초기화는 파일 삭제가 아니라 파일을 유지한 채 audio data 영역과 필요 시 metadata를 초기화하는 동작이다. 이 요청은 상태 관리 태스크가 직접 저장 장치 입출력 태스크에 보낸다.

```mermaid
sequenceDiagram
    participant Control as 사용자 컨트롤 처리 태스크
    participant State as 루프스테이션 상태 관리 태스크
    participant Storage as 저장 장치 입출력 태스크
    participant Display as LED/디스플레이 태스크

    Control->>State: CONTROL_BUTTON(reset gesture)
    State->>State: 트랙 초기화 조건 확인
    State->>Storage: TRACK_FILE_RESET(request_id, track_id, reset_audio_data, reset_metadata)
    alt 초기화 성공
        Storage-->>State: TRACK_FILE_RESET_DONE(track_id)
        State->>State: canonical track state 초기화
        State->>Display: TRACK_STATE_SNAPSHOT_RENDER(all tracks)
    else 초기화 실패
        Storage-->>State: TRACK_FILE_RESET_FAILED(track_id, error_code)
        State->>Display: SYSTEM_SETTING_RENDER 또는 DIAGNOSTIC_RENDER
    end
```

## 7. 저장 장치 상태 및 오류 보고

SD 카드 삽입, mount 상태, 남은 용량처럼 시스템 상태에 영향을 주는 정보는 상태 관리 태스크로 보고한다. 반면 오디오 처리 요청에 대한 read/write/open/close 실패는 우선 요청자인 오디오 처리 태스크로 돌아가며, 오디오 처리 태스크가 상태 전이에 필요한 오류만 `AUDIO_TRACK_IO_FAILED`로 상태 관리 태스크에 보고한다.

```mermaid
sequenceDiagram
    participant Storage as 저장 장치 입출력 태스크
    participant State as 루프스테이션 상태 관리 태스크
    participant Display as LED/디스플레이 태스크

    Storage-->>State: STORAGE_MEDIA_STATUS(inserted, mounted, free_bytes, total_bytes)
    State->>State: storage canonical state 갱신
    alt 현재 화면이 저장 장치 상태를 표시 중
        State->>Display: SYSTEM_SETTING_RENDER(storage status)
    else 표시 중인 화면과 무관
        State->>State: 표시 메시지 생략
    end

    opt 저장 장치 공통 오류 발생
        Storage-->>State: STORAGE_ERROR(operation, error_code)
        State->>State: 오류 상태 갱신
        State->>Display: SYSTEM_SETTING_RENDER 또는 DIAGNOSTIC_RENDER
    end
```

## 8. 메시지 전달 기준

| 상황 | 요청 송신 | 결과 수신 | 기준 |
| --- | --- | --- | --- |
| 녹음 파일 open/write/close | 오디오 처리 태스크 | 오디오 처리 태스크 | 파일은 read-write 모드로 열되, 녹음 중에는 write 요청만 사용한다. 오디오 처리에 필요한 I/O이므로 상태 관리 태스크를 경유하지 않는다. |
| 반복 재생 파일 open/read/close | 오디오 처리 태스크 | 오디오 처리 태스크 | 파일은 read-write 모드로 열되, 재생 중에는 read 요청만 사용한다. loop 위치는 다음 read chunk의 `file_frame_offset`으로 표현한다. |
| 오버더빙 read/write | 오디오 처리 태스크 | 오디오 처리 태스크 | 하나의 read-write handle로 기존 트랙을 읽고 합성 결과를 같은 파일 offset에 다시 쓴다. |
| 트랙 파일 초기화 | 루프스테이션 상태 관리 태스크 | 루프스테이션 상태 관리 태스크 | 사용자의 트랙 초기화 요청은 상태 모델 변경을 동반한다. |
| SD 카드 상태/공통 오류 | 저장 장치 입출력 태스크 | 루프스테이션 상태 관리 태스크 | 시스템 전체 상태와 UI 표시 정책에 영향을 준다. |

## 9. 남은 결정 사항

- `request_id` 생성 주체와 중복 방지 정책을 결정한다.
- `STORAGE_CHUNK_IO_FAILED` 수신 후 오디오 처리 태스크가 retry, silence fill, 즉시 정지 중 어떤 복구 정책을 사용할지 결정한다.
- 저장 장치 입출력 태스크의 queue 길이와 chunk 요청 backpressure 정책을 결정한다.
