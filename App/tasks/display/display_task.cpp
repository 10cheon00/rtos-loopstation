#include "display_task.h"

#include <array>
#include <cstdbool>

#include "cmsis_os2.h"
#include "display_initparams.h"
#include "display_messages.h"
#include "enum_map.hpp"
#include "fx_slot.hpp"
#include "gmg12864_lcd.hpp"
#include "mcp23017.hpp"
#include "mcp23017_gpio_map.hpp"
#include "page.hpp"
#include "parameter.hpp"
#include "system_init_event_flag.hpp"
#include "track_state_id.hpp"
#include "ui_renderer.hpp"
#include "ui_state_label_map.hpp"
#include "utils.hpp"
#include "xl5050_rgbw.hpp"

#define DISPLAY_RENDER_FREQEUNCY_HZ (100UL)
#define DISPLAY_RENDER_DELAY_MS (1000UL / DISPLAY_RENDER_FREQEUNCY_HZ)
#define DISPLAY_RENDER_DELAY_TICKS (pdMS_TO_TICKS(DISPLAY_RENDER_DELAY_MS))

static osMessageQueueId_t display_snapshot_mailbox;
static EnumMap<Fx::Slot, bool> ifx_state_snapshot;
static EnumMap<Fx::Slot, bool> tfx_state_snapshot;
static std::array<TrackStateMachine::Id, static_cast<std::size_t>(TRACK_COUNT)>
    track_state_snapshot;

struct TrackLedColorSet {
  Mcp23017::LedState red;
  Mcp23017::LedState green;
  Mcp23017::LedState blue;
};

static constexpr EnumMap<TrackStateMachine::Id, TrackLedColorSet>
    track_state_led_color_map{
        {TrackStateMachine::Id::IDLE,
         (TrackLedColorSet){
             Mcp23017::LedState::OFF,
             Mcp23017::LedState::OFF,
             Mcp23017::LedState::OFF,
         }},
        {TrackStateMachine::Id::RECORDING,
         (TrackLedColorSet){
             Mcp23017::LedState::ON,
             Mcp23017::LedState::OFF,
             Mcp23017::LedState::OFF,
         }},
        {TrackStateMachine::Id::STOPPED,
         (TrackLedColorSet){
             Mcp23017::LedState::OFF,
             Mcp23017::LedState::OFF,
             Mcp23017::LedState::ON,
         }},
        {TrackStateMachine::Id::PLAYING,
         (TrackLedColorSet){
             Mcp23017::LedState::OFF,
             Mcp23017::LedState::ON,
             Mcp23017::LedState::OFF,
         }},
        {TrackStateMachine::Id::OVERDUBBING,
         (TrackLedColorSet){
             Mcp23017::LedState::ON,
             Mcp23017::LedState::ON,
             Mcp23017::LedState::OFF,
         }},
};

static void Run(void);
static TaskStatus HandlePanelRenderPayload(RtosPayload_PanelRender& payload);
static TaskStatus HandleLedRenderPayload(RtosPayload_LedRender& payload);
static TaskStatus RenderFxLed(bool is_on, Mcp23017::GpioId gpio_id);
static TaskStatus RenderTrackLed(TrackStateMachine::Id state_id,
                                 TrackIndex track_index);
static void UpdateFxLedStateIfChanged(RtosPayload_LedRender& payload);
static void UpdateTrackLedStateSnapshotIfChanged(
    RtosEnumValue* rtos_enum_value_track_states);

static int IsValidInitParams(const DisplayInitParams* params) {
  return (params != 0) &&
         (params->display_snapshot_mailbox != 0 && params->hspi != NULL &&
          params->CS_Pin != 0 && params->CS_Port != NULL &&
          params->RST_Pin != 0 && params->RST_Port != NULL &&
          params->DC_Pin != 0 && params->DC_Port != NULL &&
          params->hi2c != NULL && params->i2c1_mutex != 0 &&
          params->system_init_event != 0);
}

void DisplayTask_Init(void* argument) {
  const DisplayInitParams* params = (const DisplayInitParams*)argument;

  if (!IsValidInitParams(params)) {
    // TODO: 초기화 단계에서 오류 발생 시 처리 흐름에 대해 요구사항에서 정의하기
    for (;;) {
      osDelay(1);
    }
  }

  ifx_state_snapshot[Fx::Slot::A] = false;
  ifx_state_snapshot[Fx::Slot::B] = false;
  ifx_state_snapshot[Fx::Slot::C] = false;
  tfx_state_snapshot[Fx::Slot::A] = false;
  tfx_state_snapshot[Fx::Slot::B] = false;
  tfx_state_snapshot[Fx::Slot::C] = false;
  for (std::size_t i = 0; i < TRACK_COUNT; i++) {
    track_state_snapshot[i] = TrackStateMachine::Id::IDLE;
  }

  display_snapshot_mailbox = params->display_snapshot_mailbox;

  osEventFlagsWait(params->system_init_event, SystemInitEventFlag::Inited,
                   osFlagsWaitAll | osFlagsNoClear, osWaitForever);
  Run();
}

static void Run(void) {
  TickType_t last_wake_ticks = 0, next_wake_ticks;
  RtosMessage_DisplaySnapshot snapshot;

  for (;;) {
    next_wake_ticks = last_wake_ticks + DISPLAY_RENDER_DELAY_TICKS;
    osDelayUntil(next_wake_ticks);
    last_wake_ticks = osKernelGetTickCount();
    osMessageQueueGet(display_snapshot_mailbox, &snapshot, NULL, 0);
    HandlePanelRenderPayload(snapshot.panel);
    HandleLedRenderPayload(snapshot.led);
  }
}

static TaskStatus HandlePanelRenderPayload(
    RtosPayload_PanelRender& panel_render_payload) {
  UiRenderer::DrawPanelLayout(
      panel_render_payload.label,
      PageNavigationBitset(
          panel_render_payload.rtos_enum_value_page_navigation_bitset));

  for (std::uint8_t i = 0; i < static_cast<std::uint8_t>(SlotIndex::COUNT);
       i++) {
    const SlotIndex slot_index = static_cast<SlotIndex>(i);
    RtosPayload_PageSlotRender* payload = &panel_render_payload.page_slots[i];
    PageSlotType type = Utils::FromRtosEnumValue<PageSlotType>(
        payload->rtos_enum_value_page_slot_type);
    if (type == PageSlotType::MENU) {
      RtosPayload_MenuRender* menu_render_payload = &payload->data.menu;
      MenuIconEncoding icon_encoding =
          Utils::FromRtosEnumValue<MenuIconEncoding>(
              menu_render_payload->rtos_enum_value16_menu_icon_encoding);
      UiRenderer::DrawMenu(icon_encoding, menu_render_payload->label,
                           slot_index);
    } else if (type == PageSlotType::PARAMETER) {
      RtosPayload_ParameterRender* parameter_render_payload =
          &payload->data.parameter;
      ParameterModel::Parameter parameter{
          parameter_render_payload->rtos_parameter_copy};
      UiRenderer::DrawParameter(parameter, parameter_render_payload->label,
                                slot_index);
    }
  }

  Gmg12864::Driver::GetInstance().sendBuffer();

  return TASK_STATUS_OK;
}

static TaskStatus HandleLedRenderPayload(RtosPayload_LedRender& payload) {
  // UpdateFxLedStateIfChanged(payload);
  UpdateTrackLedStateSnapshotIfChanged(payload.rtos_enum_value_track_states);

  return TASK_STATUS_OK;
}

static void UpdateFxLedStateIfChanged(RtosPayload_LedRender& payload) {
  // ParameterModel::Parameter ifx_a_state{
  //     payload.rtos_parameter_copy_ifx_a_state};
  // ParameterModel::Parameter tfx_a_state{
  //     payload.rtos_parameter_copy_tfx_a_state};

  // if (ifx_state_snapshot[Fx::Slot::A] !=
  //     ifx_a_state.GetValue<ParameterModel::ToggleValue>().is_on) {
  //   ifx_state_snapshot[Fx::Slot::A] =
  //       ifx_a_state.GetValue<ParameterModel::ToggleValue>().is_on;
  //   RenderFxLed(ifx_state_snapshot[Fx::Slot::A],
  //   Mcp23017::GpioId::LED_IFX_A);
  // }
  // if (tfx_state_snapshot[Fx::Slot::A] !=
  //     tfx_a_state.GetValue<ParameterModel::ToggleValue>().is_on) {
  //   tfx_state_snapshot[Fx::Slot::A] =
  //       tfx_a_state.GetValue<ParameterModel::ToggleValue>().is_on;
  //   RenderFxLed(tfx_state_snapshot[Fx::Slot::A],
  //   Mcp23017::GpioId::LED_TFX_A);
  // }
}

static void UpdateTrackLedStateSnapshotIfChanged(
    RtosEnumValue* rtos_enum_value_track_states) {
  for (std::size_t i = 0; i < TRACK_COUNT; i++) {
    TrackStateMachine::Id id = Utils::FromRtosEnumValue<TrackStateMachine::Id>(
        rtos_enum_value_track_states[i]);
    if (track_state_snapshot[i] != id) {
      track_state_snapshot[i] = id;
      RenderTrackLed(id, static_cast<TrackIndex>(i));
    }
  }
}

// ParameterModel::Id에 매핑된 address, port, 레지스터 상 핀의 비트 위치를
// 찾아야 함 현재 핀 상태에 따라 수정된 핀의 값을 Mcp23017 드라이버에게 넘겨
// 값을 업데이트하라고 함
static TaskStatus RenderFxLed(bool is_on, Mcp23017::GpioId gpio_id) {
  //   Mcp23017::Driver& driver = Mcp23017::Driver::GetInstance();
  //   const Mcp23017::PinConfigMap& pin_config_map =
  //   Mcp23017::GetPinConfigMap(); const Mcp23017::PinConfig& pin_config =
  //   pin_config_map.Get(gpio_id); Mcp23017::LedState pin_state =
  //       is_on ? Mcp23017::LedState::ON : Mcp23017::LedState::OFF;

  //   if (driver.UpdateLedState(pin_config.address, gpio_id, pin_state) !=
  //       Mcp23017::Status::OK) {
  //     return TASK_STATUS_ERROR;
  //   }

  return TASK_STATUS_OK;
}

static TaskStatus RenderTrackLed(TrackStateMachine::Id state_id,
                                 TrackIndex track_index) {
  Xl5050Rgbw::Driver& driver = Xl5050Rgbw::Driver::GetInstance();
  driver.UpdateTrackLedStatus(state_id, track_index);
  return TASK_STATUS_OK;
}
