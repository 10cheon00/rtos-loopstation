#include "audio_task.h"

#include <cstdbool>

#include "audio_initparams.h"
#include "cmsis_os2.h"
#include "system_init_event_flag.hpp"

static bool IsValidParams(AudioInitParams* params);
static void Run();

void AudioTask_Init(void* argument) {
  AudioInitParams* params = (AudioInitParams*)argument;

  if (!IsValidParams(params)) {
    for (;;) {
      osDelay(1);
    }
  }

  osEventFlagsWait(params->system_init_event, SystemInitEventFlag::Inited,
                   osFlagsWaitAll | osFlagsNoClear, osWaitForever);
  Run();
}

static void Run() {
  for (;;) {
    osDelay(1);
  }
}

bool IsValidParams(AudioInitParams* params) {
  return params != NULL && params->hsai != NULL &&
         params->system_init_event != NULL;
}
