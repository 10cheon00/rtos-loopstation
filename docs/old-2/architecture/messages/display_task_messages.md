---
title: LED/디스플레이 처리 태스크 수신 메시지 스키마
version: 0.5.0
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: LED/디스플레이 처리 태스크가 수신하는 command와 telemetry 메시지 스키마를 정리함
  - date: 2026-07-10
    version: 0.2.0
    summary: FX와 트랙의 상태 표시 메시지에서 파라미터 값 표시 메시지를 분리함
  - date: 2026-07-10
    version: 0.2.1
    summary: UI_STATE_RENDER를 화면 전환 전용 표시 메시지로 명확히 함
  - date: 2026-07-10
    version: 0.3.0
    summary: 표시용 mailbox를 track_state, track_param, fx_state, fx_param, system, diagnostic으로 분리함
  - date: 2026-07-10
    version: 0.4.0
    summary: FX와 트랙 파라미터 표시 payload를 단일 값이 아닌 전체 snapshot 구조로 변경함
  - date: 2026-07-10
    version: 0.5.0
    summary: 트랙 상태와 FX 상태 표시 payload를 전체 snapshot 구조로 변경함
---

# LED/디스플레이 처리 태스크 수신 메시지 스키마

이 문서는 LED/디스플레이 처리 태스크가 수신하는 표시 command와 telemetry 메시지를 정의한다.

## 1. Queue

| Queue | 수신 방식 | 설명 |
| --- | --- | --- |
| `display_command_queue` | 일반 queue | 초기화, 화면 전환, 밝기 변경처럼 누락되면 안 되는 표시 명령을 수신한다. |
| `track_state_mailbox` | overwritable queue(mailbox) | 모든 트랙의 최신 상태 snapshot을 수신한다. |
| `track_param_mailbox` | overwritable queue(mailbox) | 최신 트랙 파라미터 snapshot을 수신한다. |
| `fx_state_mailbox` | overwritable queue(mailbox) | IFX와 TFX의 최신 상태 snapshot을 수신한다. |
| `fx_param_mailbox` | overwritable queue(mailbox) | 최신 FX 파라미터 snapshot을 수신한다. |
| `system_mailbox` | overwritable queue(mailbox) | 최신 시스템 설정과 master gain 표시값을 수신한다. |
| `diagnostic_mailbox` | overwritable queue(mailbox) | 최신 하드웨어 점검 표시값을 수신한다. |
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
        TrackStateSnapshotRenderPayload track_state_snapshot;
        FxStateSnapshotRenderPayload fx_state_snapshot;
        FxParamSnapshotRenderPayload fx_param_snapshot;
        TrackParamSnapshotRenderPayload track_param_snapshot;
        MasterGainRenderPayload master_gain;
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
| UI 화면 전환 | 루프스테이션 상태 관리 태스크 | `UI_STATE_RENDER` | `display_command_queue` | `UiStateRenderPayload` | 패널이 바뀔 때 새 패널 화면을 표시한다. |
| 트랙 상태 snapshot 표시 | 루프스테이션 상태 관리 태스크 | `TRACK_STATE_SNAPSHOT_RENDER` | `track_state_mailbox` | `TrackStateSnapshotRenderPayload` | 모든 트랙의 record/play/overdub/stop 상태와 LED 표시를 갱신한다. |
| 트랙 파라미터 snapshot 표시 | 루프스테이션 상태 관리 태스크 | `TRACK_PARAM_SNAPSHOT_RENDER` | `track_param_mailbox` | `TrackParamSnapshotRenderPayload` | 트랙 볼륨, TFX 적용 여부, 재생 방향 같은 트랙 설정값 전체를 갱신한다. |
| 트랙 위치 표시 | 오디오 처리 태스크 | `AUDIO_TRACK_POSITION` | `track_position_mailbox` | `AudioTrackPositionPayload` | 현재 재생/녹음 위치를 직접 전달한다. |
| FX 상태 snapshot 표시 | 루프스테이션 상태 관리 태스크 | `FX_STATE_SNAPSHOT_RENDER` | `fx_state_mailbox` | `FxStateSnapshotRenderPayload` | IFX와 TFX의 선택, 활성화 상태, LED 표시를 갱신한다. |
| FX 파라미터 snapshot 표시 | 루프스테이션 상태 관리 태스크 | `FX_PARAM_SNAPSHOT_RENDER` | `fx_param_mailbox` | `FxParamSnapshotRenderPayload` | 현재 FX 패널을 그리는 데 필요한 파라미터 값 전체를 갱신한다. |
| master gain 표시 | 루프스테이션 상태 관리 태스크 | `MASTER_GAIN_RENDER` | `system_mailbox` | `MasterGainRenderPayload` | master gain 값 표시를 갱신한다. |
| 시스템 설정 표시 | 루프스테이션 상태 관리 태스크 | `SYSTEM_SETTING_RENDER` | `system_mailbox` | `SystemSettingRenderPayload` | 시스템 설정 패널 표시값을 갱신한다. |
| LCD 밝기 변경 | 루프스테이션 상태 관리 태스크 | `LCD_BRIGHTNESS_SET` | `display_command_queue` | `LcdBrightnessPayload` | LCD 밝기를 변경한다. |
| 레벨 미터 표시 | 오디오 처리 태스크 | `AUDIO_LEVEL_METER` | `level_meter_mailbox` | `AudioLevelMeterPayload` | 입력/출력 레벨 표시값을 직접 전달한다. |
| 하드웨어 점검 표시 | 루프스테이션 상태 관리 태스크 | `DIAGNOSTIC_RENDER` | `diagnostic_mailbox` | `DiagnosticRenderPayload` | 버튼, LED, 포텐셔미터, 로터리 엔코더 검사 패널을 표시한다. |

## 4. Payload 스키마

### 4.1 `DisplayInitPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `initial_panel` | `PanelId` | 초기 표시 패널 |
| `brightness` | `uint8_t` | 초기 LCD 밝기 |

### 4.2 `UiStateRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `panel_id` | `PanelId` | 전환할 대상 패널 |
| `selected_track` | `uint8_t` | 선택된 트랙 |
| `selected_item` | `uint16_t` | 새 패널 진입 시 초기에 강조할 항목. 패널 내부 값 변경에는 사용하지 않는다. |

### 4.3 `TrackStateSnapshotRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `selected_track` | `uint8_t` | 현재 선택 또는 강조할 트랙 |
| `track_count` | `uint8_t` | snapshot에 포함된 트랙 개수 |
| `tracks` | `TrackStateDisplayValue[]` | 모든 트랙의 상태, 길이, LED 표시값 목록 |

`track_state_mailbox`는 overwrite될 수 있으므로 단일 트랙 상태만 보내지 않는다. 트랙 A 상태 갱신 직후 트랙 B 상태 갱신이 들어와 A 메시지가 덮어써져도 화면 일관성이 깨지지 않도록, 매번 모든 트랙의 상태 snapshot을 보낸다.

### 4.4 `AudioTrackPositionPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `position_frame` | `uint32_t` | 현재 재생/녹음 위치 |
| `track_length_frames` | `uint32_t` | 트랙 전체 길이 |
| `state` | `TrackState` | 표시 시점의 오디오 로컬 상태 |

### 4.5 `FxStateSnapshotRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `selected_fx_bus` | `FxBus` | 현재 선택 또는 강조할 FX bus |
| `fx_count` | `uint8_t` | snapshot에 포함된 FX 상태 개수 |
| `fx_states` | `FxStateDisplayValue[]` | IFX와 TFX의 선택 FX, 활성화 상태, LED 표시값 목록 |

`fx_state_mailbox`도 overwrite될 수 있으므로 IFX 또는 TFX 하나의 상태만 보내지 않는다. IFX와 TFX 상태가 짧은 간격으로 바뀌어도 최신 snapshot 하나로 FX 상태 영역을 일관되게 다시 그릴 수 있어야 한다.

### 4.6 `FxParamSnapshotRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `fx_bus` | `FxBus` | IFX 또는 TFX |
| `track_id` | `uint8_t` | TFX 대상 트랙. IFX 또는 global이면 `TRACK_ID_NONE` |
| `fx_id` | `FxId` | FX 종류 |
| `selected_param_id` | `FxParamId` | 현재 선택 또는 강조할 FX 파라미터 |
| `param_count` | `uint8_t` | snapshot에 포함된 파라미터 개수 |
| `params` | `FxParamDisplayValue[]` | 현재 FX 패널 표시를 구성하는 파라미터 ID와 값 목록 |

`fx_param_mailbox`는 overwrite될 수 있으므로 단일 파라미터 변경값만 보내지 않는다. 파라미터 A 갱신 직후 파라미터 B 갱신이 들어와 A 메시지가 덮어써져도 화면 일관성이 깨지지 않도록, 매번 현재 FX 패널을 그릴 수 있는 전체 파라미터 snapshot을 보낸다.

### 4.7 `TrackParamSnapshotRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `selected_param_id` | `TrackParamId` | 현재 선택 또는 강조할 트랙 파라미터 |
| `param_count` | `uint8_t` | snapshot에 포함된 파라미터 개수 |
| `params` | `TrackParamDisplayValue[]` | track gain, TFX enable, play direction 등 트랙 설정 항목과 값 목록 |

`track_param_mailbox`도 overwrite될 수 있으므로 단일 트랙 파라미터 변경값만 보내지 않는다. 트랙 설정 패널에 표시되는 값들이 서로 덮어써져 누락되지 않도록, 매번 현재 트랙 설정 패널을 그릴 수 있는 전체 파라미터 snapshot을 보낸다.

### 4.7.1 `FxParamDisplayValue`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `param_id` | `FxParamId` | FX 파라미터 ID |
| `value` | `int32_t` | 표시값 |

### 4.7.2 `TrackParamDisplayValue`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `param_id` | `TrackParamId` | 트랙 파라미터 ID |
| `value_i32` | `int32_t` | 정수 표시값 |
| `value_u32` | `uint32_t` | unsigned 표시값 또는 bitmask |

### 4.7.3 `TrackStateDisplayValue`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `track_id` | `uint8_t` | 대상 트랙 |
| `track_state` | `TrackState` | stopped, recording, playing, overdubbing 등 |
| `track_length_frames` | `uint32_t` | 확정된 트랙 길이 |
| `led_state` | `LedState` | 트랙 LED 표시 상태 |

### 4.7.4 `FxStateDisplayValue`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `fx_bus` | `FxBus` | IFX 또는 TFX |
| `track_id` | `uint8_t` | TFX 대상 트랙. IFX 또는 global이면 `TRACK_ID_NONE` |
| `fx_id` | `FxId` | FX 종류 |
| `enabled` | `bool` | 활성화 여부 |
| `led_state` | `LedState` | FX LED 표시 상태 |

### 4.8 `MasterGainRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `value` | `int32_t` | master gain 표시값 |

### 4.9 `SystemSettingRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `setting_id` | `SystemSettingId` | 설정 항목 |
| `value` | `int32_t` | 표시값 |

### 4.10 `LcdBrightnessPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `brightness` | `uint8_t` | LCD 밝기 |

### 4.11 `AudioLevelMeterPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `input_level_l` | `int16_t` | 입력 L 레벨 |
| `input_level_r` | `int16_t` | 입력 R 레벨 |
| `output_level_l` | `int16_t` | 출력 L 레벨 |
| `output_level_r` | `int16_t` | 출력 R 레벨 |
| `clip_flags` | `uint32_t` | clipping 표시 bitmask |

### 4.12 `DiagnosticRenderPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `diagnostic_item` | `DiagnosticItemId` | 검사 항목 |
| `value` | `int32_t` | 검사 표시값 |
| `status` | `DiagnosticStatus` | OK, warning, error 등 |
