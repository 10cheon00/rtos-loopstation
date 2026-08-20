#include "audio_task.h"

#include <stdbool.h>
#include <string.h>

#include "cmsis_os2.h"
#include "stm32h7xx.h"

#include "audio_initparams.h"
#include "audio_messages.h"

#define AUDIO_FRAME_COUNT 128U
#define AUDIO_CHANNEL_COUNT 2U
#define AUDIO_WORD_COUNT (AUDIO_FRAME_COUNT * AUDIO_CHANNEL_COUNT)
#define AUDIO_HALF_WORD_COUNT (AUDIO_WORD_COUNT / 2U)
#define AUDIO_BUFFER_HALF_COUNT 2U

/*
 * INMP441의 L/R 핀이 GND이면 left slot(0), VDD이면 right slot(1)을 선택한다.
 * 보드 배선이 반대라면 이 값만 1U로 변경한다.
 */
#define INMP441_CHANNEL_INDEX 0U

typedef struct {
    bool rx_complete;
    bool tx_complete;
} AudioDmaHalfState;

static AudioDmaHalfState dma_half_state[AUDIO_BUFFER_HALF_COUNT];
static SAI_HandleTypeDef *hsai_tx;
static SAI_HandleTypeDef *hsai_rx;
static osMessageQueueId_t audio_event_queue;

/**
 * Rx 버퍼에는 SAI RX에서 들어온 PCM 데이터가 담긴다. DMA가 이 버퍼의 절반을 채웠을 때 인터럽트가
 * 발생하고, 채워진 버퍼의 절반을 Tx 버퍼로 옮긴다.
 * Tx 버퍼를 읽을 DMA와 같은 영역을 읽지 않도록, 각 버퍼의 절반마다 플래그 변수를 두고 해당 버퍼의
 * Rx와 Tx 플래그가 모두 활성화되어 있을 때 Rx 버퍼의 값을 Tx 버퍼의 값으로 옮기도록 한다.
 * DMA를 쓰므로, SAI가 속한 D2 도메인에 있는 SRAM에 버퍼를 할당해야함.
 */
__attribute__((section(".ram_d2"), aligned(32))) static uint32_t rx_buffer[AUDIO_WORD_COUNT];
__attribute__((section(".ram_d2"), aligned(32))) static uint32_t tx_buffer[AUDIO_WORD_COUNT];

static bool IsValidInitParams(const AudioInitParams *params);
static void CopyMicrophoneToStereo(size_t offset, size_t word_count);
static void HandleSaiDmaCallback(SaiDmaEventType event_type);
static void TryProcessHalf(size_t half_index);
static void Run(void);

void AudioTask_Init(void *arguments)
{
    const AudioInitParams *params = (const AudioInitParams *)arguments;

    if (!IsValidInitParams(params)) {
        for (;;) {
            osDelay(1U);
        }
    }

    hsai_tx = params->hsaiTx;
    hsai_rx = params->hsaiRx;
    audio_event_queue = params->audio_event_queue;

    Run();
}

static bool IsValidInitParams(const AudioInitParams *params)
{
    return params != NULL && params->audio_event_queue != NULL && params->hsaiTx != NULL &&
           params->hsaiRx != NULL;
}

static void Run(void)
{
    AudioEvent audio_event;

    /* Synchronous RX가 master TX의 BCLK/LRCK를 사용하므로 RX를 먼저 arm한다. */
    if (HAL_SAI_Receive_DMA(hsai_rx, (uint8_t *)rx_buffer, AUDIO_WORD_COUNT) != HAL_OK) {
        for (;;) {
            osDelay(1U);
        }
    }
    if (HAL_SAI_Transmit_DMA(hsai_tx, (uint8_t *)tx_buffer, AUDIO_WORD_COUNT) != HAL_OK) {
        (void)HAL_SAI_DMAStop(hsai_rx);
        for (;;) {
            osDelay(1U);
        }
    }

    for (;;) {
        if (osMessageQueueGet(audio_event_queue, &audio_event, NULL, osWaitForever) != osOK) {
            continue;
        }
        if (audio_event.type != AUDIO_EVENT_TYPE_SAI_DMA_CALLBACK) {
            continue;
        }

        HandleSaiDmaCallback(audio_event.payload.sai_dma_event.type);
    }
}

static void HandleSaiDmaCallback(SaiDmaEventType event_type)
{
    switch (event_type) {
    case SAI_DMA_RX_HALF_COMPLETE:
        dma_half_state[0].rx_complete = true;
        TryProcessHalf(0U);
        break;
    case SAI_DMA_TX_HALF_COMPLETE:
        dma_half_state[0].tx_complete = true;
        TryProcessHalf(0U);
        break;
    case SAI_DMA_RX_COMPLETE:
        dma_half_state[1].rx_complete = true;
        TryProcessHalf(1U);
        break;
    case SAI_DMA_TX_COMPLETE:
        dma_half_state[1].tx_complete = true;
        TryProcessHalf(1U);
        break;
    default:
        break;
    }
}

static void TryProcessHalf(size_t half_index)
{
    AudioDmaHalfState *state = &dma_half_state[half_index];

    if (!state->rx_complete || !state->tx_complete) {
        return;
    }

    CopyMicrophoneToStereo(half_index * AUDIO_HALF_WORD_COUNT, AUDIO_HALF_WORD_COUNT);
    state->rx_complete = false;
    state->tx_complete = false;
}

static void CopyMicrophoneToStereo(size_t offset, size_t word_count)
{
    const size_t end = offset + word_count;

    for (size_t index = offset; index < end; index += AUDIO_CHANNEL_COUNT) {
        /*
         * RX와 TX를 모두 24-bit I2S/32-bit slot으로 설정했으므로 INMP441에서
         * 수신한 24-bit two's-complement sample을 별도 shift 없이 전달한다.
         */
        const uint32_t sample = rx_buffer[index + INMP441_CHANNEL_INDEX];
        tx_buffer[index] = sample;
        tx_buffer[index + 1U] = sample;
    }
}
