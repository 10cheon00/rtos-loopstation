#ifndef AUDIO_EVENT_SENDER_H
#define AUDIO_EVENT_SENDER_H

#include <stdbool.h>
#include <stdint.h>

#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C" {
#endif

void AudioDmaEvent_SendSaiRxHalfCplt(osMessageQueueId_t queue);
void AudioDmaEvent_SendSaiTxHalfCplt(osMessageQueueId_t queue);
void AudioDmaEvent_SendSaiRxCplt(osMessageQueueId_t queue);
void AudioDmaEvent_SendSaiTxCplt(osMessageQueueId_t queue);
void AudioDmaEvent_SendSaiError(osMessageQueueId_t queue);

#ifdef __cplusplus
}
#endif

#endif
