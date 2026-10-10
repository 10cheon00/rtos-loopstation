#include "audio_task.h"

#include <cstdbool>
#include <cstdint>

#include "FreeRTOS.h"
#include "app.h"
#include "audio_dma_event.hpp"
#include "audio_initparams.h"
#include "audio_messages.h"
#include "cmsis_os2.h"
#include "parameter.hpp"
#include "queue.h"
#include "system_init_event_flag.hpp"
#include "track_state.hpp"
#include "utils.h"

#define SAMPLING_FREQUENCY 44100UL
#define FRAME_DURATION_MS (1000.0 / SAMPLING_FREQUENCY)
#define FRAME_DURATION_TICKS (pdMS_TO_TICKS(FRAME_DURATION_MS))
#define FRAME_COUNT 512U
#define CHANNEL_COUNT 2U
#define FRAME_BUFFER_COUNT 2U
#define SAI_FRAME_BUFFER_SIZE (FRAME_COUNT * FRAME_BUFFER_COUNT)
#define INMP441_ALIGN_OFFSET 1

#define SDRAM_BASE_ADDRESS 0xC0000000
#define SDRAM_SIZE 0x3FFFFFF
#define TRACK_FRAME_BUFFER_SIZE \
  0x35D540  // 44100 Hz/s * 4 byte * 2 channel * 10 s

struct ErrorLog {
  uint32_t sdram_write_error;
  uint32_t sdram_write_none_error;
  uint32_t sdram_read_error;
  uint32_t sdram_read_none_error;
};

using BitDepth_t = std::int32_t;

struct SaiDmaState {
  bool is_rx_complete;
  bool is_tx_complete;
};

struct TrackPlaybackContext {
  uint32_t* sdram_base_address;
  uint32_t* sram_frame_buffer_address;
  uint32_t frame_index;        // 재생할 프레임의 번호
  uint32_t total_frame_count;  // 트랙의 길이
  std::int16_t volume;
  TrackStateMachine::Id track_state;
};

struct AudioInputContext {
  BitDepth_t* input_frame_buffer;
  BitDepth_t* output_frame_buffer;
  bool is_frame_ready;
};

static ErrorLog error_log{0, 0, 0, 0};

static osMessageQueueId_t audio_event_snapshot_mailbox;
static osMessageQueueId_t audio_dma_event_queue;
static SAI_HandleTypeDef* hsai_rx;
static SAI_HandleTypeDef* hsai_tx;
static SDRAM_HandleTypeDef* hsdram;

static SaiDmaState sai_dma_state[FRAME_BUFFER_COUNT];
static std::array<TrackPlaybackContext, TRACK_COUNT> track_playback_contexts;
static AudioInputContext audio_input_context;

__attribute__((section(".axi_sram"),
               aligned(sizeof(BitDepth_t)))) static BitDepth_t
    sai_rx_frame_buffer[SAI_FRAME_BUFFER_SIZE];
__attribute__((section(".axi_sram"),
               aligned(sizeof(BitDepth_t)))) static BitDepth_t
    sai_tx_frame_buffer[SAI_FRAME_BUFFER_SIZE];
__attribute__((section(".axi_sram"),
               aligned(sizeof(BitDepth_t)))) static BitDepth_t
    track_frame_buffer[TRACK_COUNT][SAI_FRAME_BUFFER_SIZE];

static bool IsValidParams(AudioInitParams* params);
static void Run();
static TaskStatus StartSaiReceiveAndTransmit();
static void FetchActiveTrackFrames();
static void WaitForAudioDmaEvent(
    RtosMessage_AudioDmaEvent& rtos_message_audio_dma_event);
static void HandleAudioDmaEvent(RtosMessage_AudioDmaEvent& audio_dma_event);
static void UpdateAudioInputContext();
static bool IsAudioInputFrameReady();
static void ReadAudioEventSnapshot();
static void RecordActiveTracks();
static void MixAudioFrames();
static void UpdateTrackPlaybackContext();

void AudioTask_Init(void* argument) {
  AudioInitParams* params = (AudioInitParams*)argument;

  if (!IsValidParams(params)) {
    for (;;) {
      osDelay(1);
    }
  }

  hsai_rx = params->hsai_rx;
  hsai_tx = params->hsai_tx;
  hsdram = params->hsdram;
  audio_event_snapshot_mailbox = params->audio_event_snapshot_mailbox;
  audio_dma_event_queue = params->audio_dma_event_queue;

  for (size_t i = 0; i < track_playback_contexts.size(); i++) {
    auto& context = track_playback_contexts[i];
    context.total_frame_count = 0;
    context.frame_index = 0;
    context.sdram_base_address =
        (uint32_t*)(SDRAM_BASE_ADDRESS + TRACK_FRAME_BUFFER_SIZE * i);
    context.sram_frame_buffer_address = (uint32_t*)track_frame_buffer[i];
    context.track_state = TrackStateMachine::Id::NONE;
    context.volume = 0U;
  }

  osEventFlagsWait(params->system_init_event, SystemInitEventFlag::Inited,
                   osFlagsWaitAll | osFlagsNoClear, osWaitForever);
  Run();
}

bool IsValidParams(AudioInitParams* params) {
  return params != NULL && params->hsai_tx && params->hsai_rx != NULL &&
         params->system_init_event != NULL && params->hsdram != NULL &&
         params->audio_event_snapshot_mailbox != NULL &&
         params->audio_dma_event_queue != NULL;
}

/**
 * 1. SAI B를 통해 얻은 오디오 샘플 확보
 * 2. 현재 재생중인 트랙의 오디오 샘플 확보
 * 3. 1, 2에서 얻은 오디오 샘플 믹싱
 * 4. 3의 결과를 SAI A로 출력
 * Sai의 DMA 콜백에 따라 오디오 샘플이 확보되고, 재생중인 트랙들의 오디오 샘플이
 * 확보되었다면 오디오 프로세싱 파이프라인을 통과시키기
 * state_task에선 다음 주기에 처리되어야 할 작업에 대해서 메일박스에
 * 업데이트하기만 함.
 */
static void Run() {
  RtosMessage_AudioDmaEvent rtos_message_audio_dma_event;

  StartSaiReceiveAndTransmit();

  for (;;) {
    WaitForAudioDmaEvent(rtos_message_audio_dma_event);
    HandleAudioDmaEvent(rtos_message_audio_dma_event);
    if (IsAudioInputFrameReady()) {
      ReadAudioEventSnapshot();
      FetchActiveTrackFrames();
      MixAudioFrames();
      RecordActiveTracks();
      UpdateTrackPlaybackContext();
    }
  }
}

static TaskStatus StartSaiReceiveAndTransmit() {
  if (HAL_SAI_Receive_DMA(hsai_rx, (uint8_t*)&sai_rx_frame_buffer,
                          SAI_FRAME_BUFFER_SIZE) != HAL_OK) {
    return TASK_STATUS_ERROR;
  }
  if (HAL_SAI_Transmit_DMA(hsai_tx, (uint8_t*)&sai_tx_frame_buffer,
                           SAI_FRAME_BUFFER_SIZE) != HAL_OK) {
    return TASK_STATUS_ERROR;
  }
  return TASK_STATUS_OK;
}

static void FetchActiveTrackFrames() {
  for (auto& track_playback_context : track_playback_contexts) {
    if (track_playback_context.track_state == TrackStateMachine::Id::PLAYING ||
        track_playback_context.track_state ==
            TrackStateMachine::Id::OVERDUBBING) {
      uint32_t* current_frame_sdram_address =
          track_playback_context.sdram_base_address +
          track_playback_context.frame_index * FRAME_COUNT;

      if (HAL_SDRAM_Read_DMA(hsdram, current_frame_sdram_address,
                             track_playback_context.sram_frame_buffer_address,
                             FRAME_COUNT) != HAL_OK) {
        error_log.sdram_read_error++;
      } else {
        error_log.sdram_read_none_error++;
      }
    }
  }
}

static void WaitForAudioDmaEvent(
    RtosMessage_AudioDmaEvent& rtos_message_audio_dma_event) {
  osMessageQueueGet(audio_dma_event_queue, &rtos_message_audio_dma_event, NULL,
                    osWaitForever);
}

static void HandleAudioDmaEvent(RtosMessage_AudioDmaEvent& audio_dma_event) {
  AudioDmaEvent sai_dma_event = FromRtosEnumValue<AudioDmaEvent>(
      audio_dma_event.rtos_enum_value_audio_dma_event);

  switch (sai_dma_event) {
    case AudioDmaEvent::SAI_RX_HALF_COMPLETE:
      sai_dma_state[0].is_rx_complete = true;
      break;
    case AudioDmaEvent::SAI_TX_HALF_COMPLETE:
      sai_dma_state[0].is_tx_complete = true;
      break;
    case AudioDmaEvent::SAI_RX_COMPLETE:
      sai_dma_state[1].is_rx_complete = true;
      break;
    case AudioDmaEvent::SAI_TX_COMPLETE:
      sai_dma_state[1].is_tx_complete = true;
      break;
    case AudioDmaEvent::ERROR:
    default:
      // TODO:
      // 오류 상태 예외 처리
      break;
  }

  UpdateAudioInputContext();
}

void UpdateAudioInputContext() {
  if (sai_dma_state[0].is_rx_complete && sai_dma_state[0].is_tx_complete) {
    audio_input_context.input_frame_buffer = sai_rx_frame_buffer;
    audio_input_context.output_frame_buffer = sai_tx_frame_buffer;
    sai_dma_state[0].is_rx_complete = false;
    sai_dma_state[0].is_tx_complete = false;
    audio_input_context.is_frame_ready = true;
  } else if (sai_dma_state[1].is_rx_complete &&
             sai_dma_state[1].is_tx_complete) {
    audio_input_context.input_frame_buffer = &sai_rx_frame_buffer[FRAME_COUNT];
    audio_input_context.output_frame_buffer = &sai_tx_frame_buffer[FRAME_COUNT];
    sai_dma_state[1].is_rx_complete = false;
    sai_dma_state[1].is_tx_complete = false;
    audio_input_context.is_frame_ready = true;
  } else {
    audio_input_context.is_frame_ready = false;
  }
}

bool IsAudioInputFrameReady() { return audio_input_context.is_frame_ready; }

void ReadAudioEventSnapshot() {
  RtosMessage_AudioEventSnapshot audio_event_snapshot;
  xQueuePeek((QueueHandle_t)audio_event_snapshot_mailbox, &audio_event_snapshot,
             0);
  for (std::size_t i = 0; i < track_playback_contexts.size(); i++) {
    track_playback_contexts[i].track_state =
        FromRtosEnumValue<TrackStateMachine::Id>(
            audio_event_snapshot.rtos_enum_value_track_states[i]);
    track_playback_contexts[i].volume = static_cast<std::int16_t>(
        audio_event_snapshot.rtos_parameter_track_volumes[i]);
  }
}

static void MixAudioFrames() {
  for (size_t i = 0; i < FRAME_COUNT; i += CHANNEL_COUNT) {
    BitDepth_t sample = 0, track_sample = 0;
    for (auto& context : track_playback_contexts) {
      if (context.track_state == TrackStateMachine::Id::PLAYING ||
          context.track_state == TrackStateMachine::Id::OVERDUBBING) {
        track_sample =
            (context.sram_frame_buffer_address[i + INMP441_ALIGN_OFFSET]);
        sample += ((double)track_sample / 100.0) * context.volume;
      }
    }
    sample += audio_input_context.input_frame_buffer[i + INMP441_ALIGN_OFFSET];
    audio_input_context.output_frame_buffer[i] = sample;
    audio_input_context.output_frame_buffer[i + 1] = sample;
  }
}

static void RecordActiveTracks() {
  for (auto& context : track_playback_contexts) {
    if (context.track_state == TrackStateMachine::Id::RECORDING ||
        context.track_state == TrackStateMachine::Id::OVERDUBBING) {
      uint32_t* current_frame_sdram_address =
          context.sdram_base_address + context.frame_index * FRAME_COUNT;

      if (context.track_state == TrackStateMachine::Id::RECORDING) {
        for (uint32_t j = 0; j < FRAME_COUNT; j++) {
          context.sram_frame_buffer_address[j] =
              audio_input_context.input_frame_buffer[j];
        }
      } else if (context.track_state == TrackStateMachine::Id::OVERDUBBING) {
        for (uint32_t j = 0; j < FRAME_COUNT; j++) {
          context.sram_frame_buffer_address[j] +=
              audio_input_context.input_frame_buffer[j];
        }
      }

      if (HAL_SDRAM_Write_DMA(hsdram, current_frame_sdram_address,
                              context.sram_frame_buffer_address,
                              FRAME_COUNT) != HAL_OK) {
        error_log.sdram_write_error++;
      } else {
        error_log.sdram_write_none_error++;
      }
    }
  }
}

void UpdateTrackPlaybackContext() {
  for (auto& context : track_playback_contexts) {
    switch (context.track_state) {
      case TrackStateMachine::Id::IDLE:
        context.total_frame_count = 0;
        break;
      case TrackStateMachine::Id::RECORDING:
        context.total_frame_count++;
        context.frame_index = context.total_frame_count;
        break;
      case TrackStateMachine::Id::PLAYING:
      case TrackStateMachine::Id::OVERDUBBING:
        context.frame_index =
            (context.frame_index + 1) % context.total_frame_count;
        break;
      default:
        break;
    }
  }
}
