#include "input_event_sender.h"

#include "encoder_rotation_direction.hpp"
#include "input_event_type.hpp"
#include "input_messages.h"
#include "utils.h"

static void SendEncoder(osMessageQueueId_t queue, EncoderId id,
                        bool counting_down);

void InputEvent_SendMcp23017(osMessageQueueId_t queue, uint16_t gpio_pin) {
  RtosMessage_InputEvent event{};
  event.rtos_enum_value_input_event_type =
      ToRtosEnumValue(InputEventType::MCP23017);
  event.payload.mcp23017_int_event.timestamp_ticks = osKernelGetTickCount();
  event.payload.mcp23017_int_event.gpio_pin = gpio_pin;
  osMessageQueuePut(queue, &event, 0, 0);
}

void InputEvent_SendEncoderA(osMessageQueueId_t queue, bool counting_down) {
  SendEncoder(queue, EncoderId::A, counting_down);
}

void InputEvent_SendEncoderB(osMessageQueueId_t queue, bool counting_down) {
  SendEncoder(queue, EncoderId::B, counting_down);
}

void InputEvent_SendEncoderC(osMessageQueueId_t queue, bool counting_down) {
  SendEncoder(queue, EncoderId::C, counting_down);
}

void InputEvent_SendEncoderD(osMessageQueueId_t queue, bool counting_down) {
  SendEncoder(queue, EncoderId::D, counting_down);
}

static void SendEncoder(osMessageQueueId_t queue, EncoderId id,
                        bool counting_down) {
  RtosMessage_InputEvent event{};
  event.rtos_enum_value_input_event_type =
      ToRtosEnumValue(InputEventType::ENCODER_ROTATION);
  event.payload.encoder_rotation_event.timestamp_ticks = osKernelGetTickCount();
  event.payload.encoder_rotation_event
      .rtos_enum_value_encoder_rotation_direction = ToRtosEnumValue(
      counting_down ? EncoderRotationDirection::COUNTER_CLOCKWISE
                    : EncoderRotationDirection::CLOCKWISE);
  event.payload.encoder_rotation_event.rtos_enum_value_encoder_id =
      ToRtosEnumValue(id);
  osMessageQueuePut(queue, &event, 0, 0);
}
