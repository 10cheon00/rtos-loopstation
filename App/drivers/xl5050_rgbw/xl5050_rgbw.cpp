#include "xl5050_rgbw.hpp"

#include "enum_map.hpp"

namespace Xl5050Rgbw {

#define TIMER_CCR_HIGH 228
#define TIMER_CCR_LOW 80

constexpr Color OFF = {0, 0, 0};
constexpr Color RED = {0x7F, 0, 0};
constexpr Color GREEN = {0, 0x7F, 0};
constexpr Color BLUE = {0, 0, 0x7F};
constexpr Color YELLOW = {0x7F, 0x7F, 0};

constexpr static EnumMap<TrackStateMachine::Id, Color> color_map = {
    {TrackStateMachine::Id::NONE, OFF},
    {TrackStateMachine::Id::IDLE, OFF},
    {TrackStateMachine::Id::RECORDING, RED},
    {TrackStateMachine::Id::PLAYING, GREEN},
    {TrackStateMachine::Id::OVERDUBBING, YELLOW},
    {TrackStateMachine::Id::STOPPED, BLUE},
};

__attribute__((
    section(".ram_d2"),
    aligned(sizeof(TimerCcr_t)))) static TimerCcr_t timer_ccrs[TimerCcrCount];

Status Driver::Initialize(TIM_HandleTypeDef* htim) {
  if (init_state == InitState::INITED) {
    return Status::OK;
  }
  init_state = InitState::ON_INIT;

  this->htim = htim;
  for (std::size_t i = 0; i < TimerCcrCount; i++) {
    timer_ccrs[i] = 0;
  }
  for (std::size_t i = 0; i < TRACK_COUNT; i++) {
    track_state_led_contexts[i].id = TrackStateMachine::Id::IDLE;
    track_state_led_contexts[i].green_led_timer_ccr_address =
        (uint16_t*)&timer_ccrs[i * ColorCount * ColorBitCount + 0];
    track_state_led_contexts[i].red_led_timer_ccr_address =
        (uint16_t*)&timer_ccrs[i * ColorCount * ColorBitCount + 8];
    track_state_led_contexts[i].blue_led_timer_ccr_address =
        (uint16_t*)&timer_ccrs[i * ColorCount * ColorBitCount + 16];
  }

  init_state = InitState::INITED;
  return Status::OK;
}

Status Driver::UpdateTrackLedStatus(TrackStateMachine::Id track_state_id,
                                    std::size_t track_index) {
  track_state_led_contexts[track_index].id = track_state_id;
  ConvertTrackStateIdsToTimerCcrs();
  return Send();
}

void Driver::ConvertTrackStateIdsToTimerCcrs() {
  for (auto& context : track_state_led_contexts) {
    const Color& color = color_map[context.id];
    UpdateColorValueToTimerCcr(context.red_led_timer_ccr_address, color.red);
    UpdateColorValueToTimerCcr(context.green_led_timer_ccr_address,
                               color.green);
    UpdateColorValueToTimerCcr(context.blue_led_timer_ccr_address, color.blue);
  }
}

void Driver::UpdateColorValueToTimerCcr(uint16_t* timer_ccr_address,
                                        ColorValue_t color_value) {
  for (int8_t i = ColorBitCount - 1, j = 0; i >= 0; i--, j++) {
    if ((color_value & (1 << i)) > 0) {
      timer_ccr_address[j] = TIMER_CCR_HIGH;
    } else {
      timer_ccr_address[j] = TIMER_CCR_LOW;
    }
  }
}

Status Driver::Send() {
  if (HAL_TIM_PWM_Start_DMA(htim, TIM_CHANNEL_1, (const uint32_t*)timer_ccrs,
                            static_cast<std::uint16_t>(TimerCcrCount)) !=
      HAL_OK) {
    return Status::ERROR;
  }
  return Status::OK;
}

}  // namespace Xl5050Rgbw
