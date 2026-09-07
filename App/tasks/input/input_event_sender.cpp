#include "input_event_sender.h"

#include "encoder_rotation_direction.hpp"
#include "input_event_type.hpp"
#include "input_messages.h"
#include "utils.h"

void InputEvent_SendMcp23017(osMessageQueueId_t queue, uint16_t gpio_pin) {
  RtosMessage_InputEvent event{};
  event.type_raw = ToRtosEnumValue(InputEventType::MCP23017);
  event.payload.mcp23017_int_event.timestamp_ticks = osKernelGetTickCount();
  event.payload.mcp23017_int_event.gpio_pin = gpio_pin;
  osMessageQueuePut(queue, &event, 0, 0);
}

void InputEvent_SendEncoderA(osMessageQueueId_t queue, bool counting_down) {
  RtosMessage_InputEvent event{};
  event.type_raw = ToRtosEnumValue(InputEventType::ENCODER_ROTATION);
  event.payload.encoder_rotation_event.timestamp_ticks = osKernelGetTickCount();
  event.payload.encoder_rotation_event.direction_raw = ToRtosEnumValue(
      counting_down ? EncoderRotationDirection::COUNTER_CLOCKWISE
                    : EncoderRotationDirection::CLOCKWISE);
  event.payload.encoder_rotation_event.encoder_id_raw =
      ToRtosEnumValue(EncoderId::A);
  osMessageQueuePut(queue, &event, 0, 0);
}
