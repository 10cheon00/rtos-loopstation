#include "audio_event_sender.h"

#include "audio_dma_event.hpp"
#include "audio_message.h"
#include "utils.h"

void AudioDmaEvent_SendSaiRxHalfCplt(osMessageQueueId_t queue) {
  RtosMessage_AudioDmaEvent audio_dma_event = {
      .rtos_enum_value_audio_dma_event =
          ToRtosEnumValue<AudioDmaEvent>(AudioDmaEvent::SAI_RX_HALF_COMPLETE)};
  osMessageQueuePut(queue, &audio_dma_event, 0, 0);
}

void AudioDmaEvent_SendSaiTxHalfCplt(osMessageQueueId_t queue) {
  RtosMessage_AudioDmaEvent audio_dma_event = {
      .rtos_enum_value_audio_dma_event =
          ToRtosEnumValue<AudioDmaEvent>(AudioDmaEvent::SAI_TX_HALF_COMPLETE)};
  osMessageQueuePut(queue, &audio_dma_event, 0, 0);
}

void AudioDmaEvent_SendSaiRxCplt(osMessageQueueId_t queue) {
  RtosMessage_AudioDmaEvent audio_dma_event = {
      .rtos_enum_value_audio_dma_event =
          ToRtosEnumValue<AudioDmaEvent>(AudioDmaEvent::SAI_RX_COMPLETE)};
  osMessageQueuePut(queue, &audio_dma_event, 0, 0);
}

void AudioDmaEvent_SendSaiTxCplt(osMessageQueueId_t queue) {
  RtosMessage_AudioDmaEvent audio_dma_event = {
      .rtos_enum_value_audio_dma_event =
          ToRtosEnumValue<AudioDmaEvent>(AudioDmaEvent::SAI_TX_COMPLETE)};
  osMessageQueuePut(queue, &audio_dma_event, 0, 0);
}

void AudioDmaEvent_SendSaiError(osMessageQueueId_t queue) {
  RtosMessage_AudioDmaEvent audio_dma_event = {
      .rtos_enum_value_audio_dma_event =
          ToRtosEnumValue<AudioDmaEvent>(AudioDmaEvent::ERROR)};
  osMessageQueuePut(queue, &audio_dma_event, 0, 0);
}
