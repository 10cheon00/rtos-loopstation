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

#define SAMPLE_COUNT 512U
#define SAMPLING_FREQUENCY 44100UL
#define SAMPLE_DURATION_MS (1000.0 / SAMPLING_FREQUENCY)
#define SAMPLE_DURATION_TICKS (pdMS_TO_TICKS(SAMPLE_DURATION_MS))
#define CHANNEL_COUNT 2U
#define SAI_BUFFER_SIZE (SAMPLE_COUNT * CHANNEL_COUNT)
#define INMP441_ALIGN_RIGHT 1

#define TRACK_BUFFER_BASE_ADDRESS 0xC0000000

using BitDepth_t = std::uint32_t;

struct SaiDmaState {
  bool is_rx_complete;
  bool is_tx_complete;
};

struct TrackPlaybackContext {
  uint32_t* track_buffer_address;
  uint32_t* track_sample_buffer_address;
  uint32_t position_frames;
  uint32_t loop_frame_count;
  TrackStateMachine::Id track_state;
};

struct AudioInputContext {
  uint32_t* input_sample_buffer;
  uint32_t* output_sample_buffer;
  size_t start;
  size_t end;
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

static SaiDmaState sai_dma_state[CHANNEL_COUNT];

TrackPlaybackContext track_playback_context[TRACK_COUNT];
AudioInputContext audio_input_context;

__attribute__((
    section(".axi_sram"),
    aligned(
        sizeof(BitDepth_t)))) static BitDepth_t sai_rx_buffer[SAI_BUFFER_SIZE];
__attribute__((
    section(".axi_sram"),
    aligned(
        sizeof(BitDepth_t)))) static BitDepth_t sai_tx_buffer[SAI_BUFFER_SIZE];
__attribute__((section(".axi_sram"),
               aligned(sizeof(BitDepth_t)))) static BitDepth_t
    track_sample_buffer[TRACK_COUNT][SAI_BUFFER_SIZE];

static bool IsValidParams(AudioInitParams* params);
static void UpdateTrackPlaybackContext();
static void Run();
static TaskStatus StartSaiReceiveAndTransmit();
static void GetFxParametersAndTrackState();
static void FetchTrackSamples();
static void FetchTrackSample(TrackPlaybackContext* context);
static void WaitForAudioDmaEvent(
    RtosMessage_AudioDmaEvent& rtos_message_audio_dma_event);
static void HandleAudioDmaEvent(RtosMessage_AudioDmaEvent& audio_dma_event);
static void MixAudioSamples();

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

  UpdateTrackPlaybackContext();

  osEventFlagsWait(params->system_init_event, SystemInitEventFlag::Inited,
                   osFlagsWaitAll | osFlagsNoClear, osWaitForever);
  Run();
}

static void UpdateTrackPlaybackContext() {
  // TODO:
  // 트랙 버퍼의 메모리 크기를 정해주는 규칙 세우기
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
    // GetFxParametersAndTrackState();
    // TODO:
    // 오디오 패스스루를 먼저 구현한 후 트랙 데이터를 믹싱하기
    // FetchTrackSamples();

    WaitForAudioDmaEvent(rtos_message_audio_dma_event);
    HandleAudioDmaEvent(rtos_message_audio_dma_event);
    MixAudioSamples();
  }
}

static TaskStatus StartSaiReceiveAndTransmit() {
  if (HAL_SAI_Receive_DMA(hsai_rx, (uint8_t*)&sai_rx_buffer, SAI_BUFFER_SIZE) !=
      HAL_OK) {
    return TASK_STATUS_ERROR;
  }
  if (HAL_SAI_Transmit_DMA(hsai_tx, (uint8_t*)&sai_tx_buffer,
                           SAI_BUFFER_SIZE) != HAL_OK) {
    return TASK_STATUS_ERROR;
  }
  return TASK_STATUS_OK;
}

void GetFxParametersAndTrackState() {
  RtosMessage_AudioEventSnapshot audio_event_snapshot;
  osMessageQueueGet(audio_event_snapshot_mailbox, &audio_event_snapshot, NULL,
                    0);
  for (uint8_t i = 0; i < TRACK_COUNT; i++) {
    track_playback_context[i].track_state =
        FromRtosEnumValue<TrackStateMachine::Id>(
            audio_event_snapshot.rtos_enum_value_track_states[i]);
  }
  parameter_ifx_a = Parameter(audio_event_snapshot.rtos_parameter_copy_ifx_a);
  parameter_ifx_b = Parameter(audio_event_snapshot.rtos_parameter_copy_ifx_b);
  parameter_ifx_c = Parameter(audio_event_snapshot.rtos_parameter_copy_ifx_c);
  parameter_tfx_a = Parameter(audio_event_snapshot.rtos_parameter_copy_tfx_a);
  parameter_tfx_b = Parameter(audio_event_snapshot.rtos_parameter_copy_tfx_b);
  parameter_tfx_c = Parameter(audio_event_snapshot.rtos_parameter_copy_tfx_c);
}

static void FetchTrackSamples() {
  // for (uint8_t i = 0; i < TRACK_COUNT; i++) {
  //   if (track_playback_context[i].track_state ==
  //           TrackStateMachine::Id::PLAYING ||
  //       track_playback_context[i].track_state ==
  //           TrackStateMachine::Id::OVERDUBBING) {
  //     FetchTrackSample(&track_playback_context[i]);
  //   }
  // }
}

static void FetchTrackSample(TrackPlaybackContext* context) {
  // 특정 트랙에서 가져와야할 샘플 위치 계산
  // uint32_t* sample_address = context->track_buffer_address;
  // // 샘플을 읽는 명령 실행
  // HAL_SDRAM_Read_DMA(hsdram, sample_address,
  //                    context->track_sample_buffer_address, SAMPLE_COUNT);
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
}

static void MixAudioSamples() {
  if (sai_dma_state[0].is_rx_complete && sai_dma_state[0].is_tx_complete) {
    audio_input_context.input_sample_buffer = sai_rx_buffer;
    audio_input_context.output_sample_buffer = sai_tx_buffer;
    sai_dma_state[0].is_rx_complete = false;
    sai_dma_state[0].is_tx_complete = false;
  } else if (sai_dma_state[1].is_rx_complete &&
             sai_dma_state[1].is_tx_complete) {
    audio_input_context.input_sample_buffer = &sai_rx_buffer[SAMPLE_COUNT];
    audio_input_context.output_sample_buffer = &sai_tx_buffer[SAMPLE_COUNT];
    sai_dma_state[1].is_rx_complete = false;
    sai_dma_state[1].is_tx_complete = false;
  } else {
    // TODO:
    // 아직 SAI B의 샘플을 확보하지 못한 경우 처리하기
    return;
  }

  for (size_t i = 0; i < SAMPLE_COUNT; i += CHANNEL_COUNT) {
    audio_input_context.output_sample_buffer[i] =
        audio_input_context.input_sample_buffer[i + INMP441_ALIGN_RIGHT];
    audio_input_context.output_sample_buffer[i + 1] =
        audio_input_context.input_sample_buffer[i + INMP441_ALIGN_RIGHT];
  }
}
