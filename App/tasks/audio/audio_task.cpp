#include "audio_task.h"

#include <cstdbool>
#include <cstdint>

#include "FreeRTOS.h"
#include "app.h"
#include "audio_dma_event.hpp"
#include "audio_initparams.h"
#include "audio_message.h"
#include "cmsis_os2.h"
#include "parameter.hpp"
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

using BitDepth_t = std::uint32_t;

struct SaiDmaState {
  bool is_rx_complete;
  bool is_tx_complete;
};

struct TrackPlaybackContext {
  BitDepth_t* track_buffer_address;
  BitDepth_t* current_frame_address;  // 재생할 프레임의 주소
  uint32_t position_frames;           // 재생할 프레임의 번호
  uint32_t loop_frame_count;          // 트랙의 길이
  TrackStateMachine::Id track_state;
};

struct AudioInputContext {
  BitDepth_t* input_frame_buffer;
  BitDepth_t* output_frame_buffer;
  bool is_frame_ready;
};

static osMessageQueueId_t audio_event_snapshot_mailbox;
static osMessageQueueId_t audio_dma_event_queue;
static SAI_HandleTypeDef* hsai_rx;
static SAI_HandleTypeDef* hsai_tx;
static SDRAM_HandleTypeDef* hsdram;

static Parameter parameter_ifx_a;
static Parameter parameter_ifx_b;
static Parameter parameter_ifx_c;
static Parameter parameter_tfx_a;
static Parameter parameter_tfx_b;
static Parameter parameter_tfx_c;

static SaiDmaState sai_dma_state[FRAME_BUFFER_COUNT];
static TrackPlaybackContext track_playback_context[TRACK_COUNT];
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
static void RecordActiveTracks();
static void MixAudioFrames();

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

  for (uint8_t i = 0; i < TRACK_COUNT; i++) {
    track_playback_context[i].loop_frame_count = 0;
    track_playback_context[i].position_frames = 0;
    track_playback_context[i].track_buffer_address =
        (BitDepth_t*)(SDRAM_BASE_ADDRESS + TRACK_FRAME_BUFFER_SIZE * i);
    track_playback_context[i].current_frame_address =
        track_playback_context[i].track_buffer_address;
    track_playback_context[i].track_state = TrackStateMachine::Id::NONE;
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
      FetchActiveTrackFrames();
      MixAudioFrames();
      RecordActiveTracks();
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
  for (uint8_t i = 0; i < TRACK_COUNT; i++) {
    if (track_playback_context[i].track_state ==
            TrackStateMachine::Id::PLAYING ||
        track_playback_context[i].track_state ==
            TrackStateMachine::Id::OVERDUBBING) {
      track_playback_context[i].current_frame_address =
          track_playback_context[i].track_buffer_address +
          track_playback_context[i].position_frames * FRAME_COUNT;
      HAL_SDRAM_Read_DMA(hsdram,
                         track_playback_context[i].current_frame_address,
                         track_frame_buffer[i], FRAME_COUNT);
      track_playback_context[i].position_frames =
          (track_playback_context[i].position_frames + 1) %
          track_playback_context[i].loop_frame_count;
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

static void RecordActiveTracks() {
  for (uint8_t i = 0; i < TRACK_COUNT; i++) {
    if (track_playback_context[i].track_state ==
            TrackStateMachine::Id::RECORDING ||
        track_playback_context[i].track_state ==
            TrackStateMachine::Id::OVERDUBBING) {
      HAL_SDRAM_Write_DMA(hsdram,
                          track_playback_context[i].current_frame_address,
                          audio_input_context.input_frame_buffer, FRAME_COUNT);
    }
  }
}

static void MixAudioFrames() {
  for (size_t i = 0; i < FRAME_COUNT; i += CHANNEL_COUNT) {
    BitDepth_t sample = 0;
    for (uint8_t j = 0; j < TRACK_COUNT; j++) {
      if (track_playback_context[j].track_state ==
              TrackStateMachine::Id::PLAYING ||
          track_playback_context[j].track_state ==
              TrackStateMachine::Id::OVERDUBBING) {
        sample += track_playback_context[j].current_frame_address[i];
      }
    }
    sample += audio_input_context.input_frame_buffer[i + INMP441_ALIGN_OFFSET];
    audio_input_context.output_frame_buffer[i] = sample;
    audio_input_context.output_frame_buffer[i + 1] = sample;
  }
}
