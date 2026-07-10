---
title: LED/디스플레이 처리 태스크 수신 메시지 스키마
version: 0.1.0
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: LED/디스플레이 처리 태스크가 수신하는 command와 telemetry 메시지 스키마를 정리함
---

# LED/디스플레이 처리 태스크 수신 메시지 스키마

이 문서는 LED/디스플레이 처리 태스크가 수신하는 표시 command와 telemetry 메시지를 정의한다.

## 1. Queue

| Queue | 수신 방식 | 설명 |
| --- | --- | --- |
| `display_command_queue` | 일반 queue | 초기화, 화면 전환, 밝기 변경처럼 누락되면 안 되는 표시 명령을 수신한다. |
| `display_render_mailbox` | overwritable queue(mailbox) | 최신 render state만 필요한 표시 상태를 수신한다. |
| `track_position_mailbox` | overwritable queue(mailbox) | 오디오 처리 태스크가 보낸 최신 트랙 위치를 수신한다. |
| `level_meter_mailbox` | overwritable queue(mailbox) | 오디오 처리 태스크가 보낸 최신 레벨 미터 값을 수신한다. |

## 2. 공통 envelope

```c
typedef struct {
    DisplayMessageType type;
    TaskId source;
    uint32_t timestamp_ms;
    union {
        DisplayInitPayload init;
        UiStateRenderPayload ui_state;
        TrackStateRenderPayload track_state;
        FxStateRenderPayload fx_state;
        SystemSettingRenderPayload system_setting;
        LcdBrightnessPayload brightness;
        DiagnosticRenderPayload diagnostic;
        AudioTrackPositionPayload track_position;
        AudioLevelMeterPayload level_meter;
    } payload;
} DisplayMessage;
```

## 3. 메시지 목록

| 메시지 종류 | 송신 태스크 | 메시지 타입 | Queue | Payload | 간단한 설명 |
| --- | --- | --- | --- | --- | --- |
| 초기 화면 표시 | 루프스테이션 상태 관리 태스크 | `DISPLAY_INIT` | `display_command_queue` | `DisplayInitPayload` | 부팅 완료 후 LCD를 홈 패널로 초기화한다. |
| UI 화면 전환/상태 표시 | 루프스테이션 상태 관리 태스크 | `UI_STATE_RENDER` | `display_command_queue` | `UiStateRenderPayload` | 현재 시스템 상태와 선택된 패널을 표시한다. |
| 트랙 상태 표시 | 루프스테이션 상태 관리 태스크 | `TRACK_STATE_RENDER` | `display_render_mailbox` | `TrackStateRenderPayload` | 트랙의 record/play/overdub/stop 상태와 LED 표시를 갱신한다. |
| 트랙 위치 표시 | 오디오 처리 태스크 | `AUDIO_TRACK_POSITION` | `track_position_mailbox` | `AudioTrackPositionPayload` | 현재 재생/녹음 위치를 직접 전달한다. |
| FX 상태 표시 | 루프스테이션 상태 관리 태스크 | `FX_STATE_RENDER` | `display_render_mailbox` | `FxStateRenderPayload` | FX 상태 패널과 LED 표시를 갱신한다. |
| 시스템 설정 표시 | 루프스테이션 상태 관리 태스크 | `SYSTEM_SETTING_RENDER` | `display_render_mailbox` | `SystemSettingRenderPayload` | 시스템 설정 패널 표시값을 갱신한다. |
| LCD 밝기 변경 | 루프스테이션 상태 관리 태스크 | `LCD_BRIGHTNESS_SET` | `display_command_queue` | `LcdBrightnessPayload` | LCD 밝기를 변경한다. |
| 레벨 미터 표시 | 오디오 처리 태스크 | `AUDIO_LEVEL_METER` | `level_meter_mailbox` | `AudioLevelMeterPayload` | 입력/출력 레벨 표시값을 직접 전달한다. |
| 하드웨어 점검 표시 | 루프스테이션 상태 관리 태스크 | `DIAGNOSTIC_RENDER` | `display_render_mailbox` | `DiagnosticRenderPayload` | 버튼, LED, 포텐셔미터, 로터리 엔코더 검사 패널을 표시한다. |

## 4. Payload 스키마

### 4.1 `DisplayInitPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `initial_panel` | `PanelId` | 초기 표시 패널 |
| `brightness` | `uint8_t` | 초기 LCD 밝기 |

### 4.2 `UiStateRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `panel_id` | `PanelId` | 현재 패널 |
| `selected_track` | `uint8_t` | 선택된 트랙 |
| `selected_item` | `uint16_t` | 패널 내부 선택 항목 |

### 4.3 `TrackStateRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `track_state` | `TrackState` | stopped, recording, playing, overdubbing 등 |
| `track_length_frames` | `uint32_t` | 확정된 트랙 길이 |
| `led_state` | `LedState` | 트랙 LED 표시 상태 |

### 4.4 `AudioTrackPositionPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `position_frame` | `uint32_t` | 현재 재생/녹음 위치 |
| `track_length_frames` | `uint32_t` | 트랙 전체 길이 |
| `state` | `TrackState` | 표시 시점의 오디오 로컬 상태 |

### 4.5 `FxStateRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `fx_bus` | `FxBus` | IFX 또는 TFX |
| `track_id` | `uint8_t` | TFX 대상 트랙. IFX 또는 global이면 `TRACK_ID_NONE` |
| `fx_id` | `FxId` | FX 종류 |
| `enabled` | `bool` | 활성화 여부 |
| `param_id` | `FxParamId` | 표시할 파라미터 |
| `value` | `int32_t` | 파라미터 값 |

### 4.6 `SystemSettingRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `setting_id` | `SystemSettingId` | 설정 항목 |
| `value` | `int32_t` | 표시값 |

### 4.7 `LcdBrightnessPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `brightness` | `uint8_t` | LCD 밝기 |

### 4.8 `AudioLevelMeterPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `input_level_l` | `int16_t` | 입력 L 레벨 |
| `input_level_r` | `int16_t` | 입력 R 레벨 |
| `output_level_l` | `int16_t` | 출력 L 레벨 |
| `output_level_r` | `int16_t` | 출력 R 레벨 |
| `clip_flags` | `uint32_t` | clipping 표시 bitmask |

### 4.9 `DiagnosticRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `diagnostic_item` | `DiagnosticItemId` | 검사 항목 |
| `value` | `int32_t` | 검사 표시값 |
| `status` | `DiagnosticStatus` | OK, warning, error 등 |
