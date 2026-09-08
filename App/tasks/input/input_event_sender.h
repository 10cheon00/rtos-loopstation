#ifndef INPUT_EVENT_SENDER_H
#define INPUT_EVENT_SENDER_H

#include <stdbool.h>
#include <stdint.h>

#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C" {
#endif

// ISR callers use non-blocking queue writes.
void InputEvent_SendMcp23017(osMessageQueueId_t queue, uint16_t gpio_pin);
void InputEvent_SendEncoderA(osMessageQueueId_t queue, bool counting_down);

#ifdef __cplusplus
}
#endif

#endif
