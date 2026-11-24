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

  init_state = InitState::INITED;
  return Status::OK;
}

Status Driver::UpdateTrackLedStatus(TrackStateMachine::Id track_state_id,
                                    TrackIndex track_index) {
  track_state_ids[static_cast<std::size_t>(track_index)] = track_state_id;
  ConvertTrackStateIdsToTimerCcrs();
  return Send();
}

void Driver::ConvertTrackStateIdsToTimerCcrs() {
  for (std::size_t i = 0; i < TrackCount; i++) {
    const Color& color = color_map[track_state_ids[i]];
    ConvertColorValueToTimerCcr(color.green, i * ColorCount * ColorBitCount);
    ConvertColorValueToTimerCcr(color.red, i * ColorCount * ColorBitCount + 8);
    ConvertColorValueToTimerCcr(color.blue,
                                i * ColorCount * ColorBitCount + 16);
  }
}

void Driver::ConvertColorValueToTimerCcr(ColorValue_t color_value,
                                         std::size_t base_index) {
  for (int8_t i = ColorBitCount - 1, j = 0; i >= 0; i--, j++) {
    if ((color_value & (1 << i)) > 0) {
      timer_ccrs[base_index + j] = TIMER_CCR_HIGH;
    } else {
      timer_ccrs[base_index + j] = TIMER_CCR_LOW;
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
