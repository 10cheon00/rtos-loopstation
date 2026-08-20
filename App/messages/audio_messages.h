#ifndef AUDIO_MESSAGES_H
#define AUDIO_MESSAGES_H

#define AUDIO_EVENT_QUEUE_TIMEOUT_500MS (500UL)

typedef enum {
    SAI_DMA_TX_HALF_COMPLETE,
    SAI_DMA_TX_COMPLETE,
    SAI_DMA_RX_HALF_COMPLETE,
    SAI_DMA_RX_COMPLETE,
} SaiDmaEventType;

typedef struct {
    SaiDmaEventType type;
} SaiDmaEventPayload;

typedef enum {
    AUDIO_EVENT_TYPE_SAI_DMA_CALLBACK = 0,
}AudioEventType;

typedef struct {
    AudioEventType type;
    union {
        SaiDmaEventPayload sai_dma_event;
    } payload;
} AudioEvent;   


#endif
