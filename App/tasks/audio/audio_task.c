#include "audio_task.h"

#include "audio_initparams.h"

static void Run();

void AudioTask_Init(void *arguments) {
    AudioInitParams* params = (AudioInitParams*)arguments;

    Run();
}

static void Run() {
    for (;;) {
        osDelay(1);
    }
}