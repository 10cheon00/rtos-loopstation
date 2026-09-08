#ifndef STATE_INITPARAMS_H
#define STATE_INITPARAMS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cmsis_os2.h"

typedef struct {
  osMessageQueueId_t state_event_queue;
  osMessageQueueId_t display_snapshot_mailbox;
} StateInitParams;

#ifdef __cplusplus
}
#endif

#endif
