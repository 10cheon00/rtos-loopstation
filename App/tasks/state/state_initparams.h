#ifndef STATE_INITPARAMS_H
#define STATE_INITPARAMS_H

#include "cmsis_os2.h"

typedef struct {
  osMessageQueueId_t state_event_queue;
  osMessageQueueId_t display_snapshot_mailbox;
} StateInitParams;

#endif
