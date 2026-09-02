#include "display_task.h"

#include <array>

#include "cmsis_os2.h"
#include "display_initparams.h"
#include "display_messages.h"
#include "enum_map.hpp"
#include "mcp23017.hpp"
#include "mcp23017_gpio_map.hpp"
#include "u8g2.h"
#include "ui_renderer.h"
#include "ui_state_label_config_table.h"
#include "track_state_id.hpp"
#include "utils.h"

#define DISPLAY_RENDER_FREQEUNCY_HZ (100UL)
#define DISPLAY_RENDER_DELAY_MS (1000UL / DISPLAY_RENDER_FREQEUNCY_HZ)
#define DISPLAY_RENDER_DELAY_TICKS (pdMS_TO_TICKS(DISPLAY_RENDER_DELAY_MS))

static u8g2_t u8g2;
static osMessageQueueId_t display_snapshot_mailbox;

struct TrackLedColorSet {
  Mcp23017LedState red;
  Mcp23017LedState green;
  Mcp23017LedState blue;
};

static constexpr EnumMap<TrackStateId, TrackLedColorSet>
    track_state_led_color_map{
        EnumEntry{TrackStateId::IDLE,
                  (TrackLedColorSet){
                      Mcp23017LedState::OFF,
                      Mcp23017LedState::OFF,
                      Mcp23017LedState::OFF,
                  }},
        EnumEntry{TrackStateId::RECORDING,
                  (TrackLedColorSet){
                      Mcp23017LedState::ON,
                      Mcp23017LedState::OFF,
                      Mcp23017LedState::OFF,
                  }},
        EnumEntry{TrackStateId::STOPPED,
                  (TrackLedColorSet){
                      Mcp23017LedState::OFF,
                      Mcp23017LedState::OFF,
                      Mcp23017LedState::ON,
                  }},
        EnumEntry{TrackStateId::PLAYING,
                  (TrackLedColorSet){
                      Mcp23017LedState::OFF,
                      Mcp23017LedState::ON,
                      Mcp23017LedState::OFF,
                  }},
        EnumEntry{TrackStateId::OVERDUBBING,
                  (TrackLedColorSet){
                      Mcp23017LedState::ON,
                      Mcp23017LedState::ON,
                      Mcp23017LedState::OFF,
                  }},
    };

static TaskStatus HandlePanelRenderPayload(PanelRenderPayload* payload);
static TaskStatus HandleLedRenderPayload(LedRenderPayload* payload);
static TaskStatus RenderFxLed(Parameter* parameter, Mcp23017GpioId gpio_id);
static TaskStatus RenderTrackLed(TrackStateId state_id, uint8_t track_index);

static int IsValidInitParams(const DisplayInitParams* params) {
  return (params != 0) &&
         (params->display_snapshot_mailbox != 0 && params->hspi != NULL &&
          params->CS_Pin != 0 && params->CS_Port != NULL &&
          params->RST_Pin != 0 && params->RST_Port != NULL &&
          params->DC_Pin != 0 && params->DC_Port != NULL);
}

void DisplayTask_Init(void* argument) {
  const DisplayInitParams* params = (const DisplayInitParams*)argument;

  if (!IsValidInitParams(params)) {
    // TODO: 초기화 단계에서 오류 발생 시 처리 흐름에 대해 요구사항에서 정의하기
    for (;;) {
      osDelay(1);
    }
  }

  display_snapshot_mailbox = params->display_snapshot_mailbox;

  Gmg12864Lcd_InitParams initparams = {
      .hspi = params->hspi,
      .CS_Pin = params->CS_Pin,
      .RST_Pin = params->RST_Pin,
      .DC_Pin = params->DC_Pin,
      .CS_Port = params->CS_Port,
      .RST_Port = params->RST_Port,
      .DC_Port = params->DC_Port,
  };

  Gmg12864LcdStatus status = Gmg12864Lcd_Init(&u8g2, &initparams);
  if (status != Gmg12864LcdStatus::OK) {
    // TODO: 초기화 단계에서 오류 발생 시 처리 흐름에 대해 요구사항에서 정의하기
    for (;;) {
      osDelay(1);
    }
  }

  DisplayTask_Run();
}

void DisplayTask_Run(void) {
  TickType_t last_wake_ticks = 0, next_wake_ticks;
  DisplaySnapshot snapshot;

  for (;;) {
    next_wake_ticks = last_wake_ticks + DISPLAY_RENDER_DELAY_TICKS;
    osDelayUntil(next_wake_ticks);
    last_wake_ticks = osKernelGetTickCount();
    osMessageQueueGet(display_snapshot_mailbox, &snapshot, NULL, 0);
    HandlePanelRenderPayload(&snapshot.panel);
    HandleLedRenderPayload(&snapshot.led);
  }
}

static TaskStatus HandlePanelRenderPayload(
    PanelRenderPayload* panel_render_payload) {
  u8g2_ClearBuffer(&u8g2);

  const char* panel_label =
      UiStateLabelConfigTable_Get(panel_render_payload->ui_state_id);
  UI_DrawPanelLayout(&u8g2, panel_label,
                     panel_render_payload->page_navigation_flag);

  for (uint8_t i = 0; i < UI_STATE_SLOT_INDEX_COUNT; i++) {
    PanelSlotRenderPayload* payload =
        &panel_render_payload->slot_render_payloads[i];
    if (payload->type == PANEL_SLOT_TYPE_MENU) {
      MenuRenderPayload* menu_render_payload = &payload->data.menu;
      UI_DrawMenu(&u8g2, menu_render_payload->icon_id,
                  menu_render_payload->label, (UiStateSlotIndex)i);
    } else if (panel_render_payload->slot_render_payloads[i].type ==
               PANEL_SLOT_TYPE_PARAMETER) {
      ParameterRenderPayload* parameter_render_payload =
          &payload->data.parameter;
      UI_DrawParameter(&u8g2, &parameter_render_payload->parameter,
                       parameter_render_payload->label, (UiStateSlotIndex)i);
    }
  }

  u8g2_SendBuffer(&u8g2);

  return TASK_STATUS_OK;
}

static TaskStatus HandleLedRenderPayload(LedRenderPayload* payload) {
  if (RenderFxLed(&payload->ifx_a_state, Mcp23017GpioId::LED_IFX_A) !=
      TASK_STATUS_OK) {
    return TASK_STATUS_ERROR;
  }
  if (RenderFxLed(&payload->tfx_a_state, Mcp23017GpioId::LED_TFX_A) !=
      TASK_STATUS_OK) {
    return TASK_STATUS_ERROR;
  }
  for (uint8_t i = 0; i < TRACK_COUNT; i++) {
    TrackStateId id;
    if (!ConvertIdRawToId<TrackStateIdRaw, TrackStateId>(payload->track_state[i], &id)) {
      return TASK_STATUS_ERROR;
    }
    if (RenderTrackLed(id, i) != TASK_STATUS_OK) {
      return TASK_STATUS_ERROR;
    }
  }
  return TASK_STATUS_OK;
}

// ParameterId에 매핑된 address, port, 레지스터 상 핀의 비트 위치를 찾아야 함
// 현재 핀 상태에 따라 수정된 핀의 값을 Mcp23017 드라이버에게 넘겨 값을
// 업데이트하라고 함
static TaskStatus RenderFxLed(Parameter* parameter, Mcp23017GpioId gpio_id) {
  Mcp23017Driver& driver = Mcp23017Driver::GetInstance();
  const Mcp23017GpioMap::PinConfigMap& pin_config_map = Mcp23017GpioMap::GetEnumMap();
  const Mcp23017GpioMap::PinConfig& pin_config = pin_config_map.Get(gpio_id);
  Mcp23017LedState pin_state = parameter->current == parameter->max
                                   ? Mcp23017LedState::ON
                                   : Mcp23017LedState::OFF;

  if (driver.UpdateLedState(pin_config.address, gpio_id, pin_state) !=
      Mcp23017Status::OK) {
    return TASK_STATUS_ERROR;
  }

  return TASK_STATUS_OK;
}

static TaskStatus RenderTrackLed(TrackStateId state_id, uint8_t track_index) {
  const Mcp23017GpioMap::PinConfig& red_entry =
      Mcp23017GpioMap::GetTrackLedEntry(track_index,
                                        Mcp23017GpioMap::TrackLedColor::RED);
  const Mcp23017GpioMap::PinConfig& green_entry =
      Mcp23017GpioMap::GetTrackLedEntry(track_index,
                                        Mcp23017GpioMap::TrackLedColor::GREEN);
  const Mcp23017GpioMap::PinConfig& blue_entry =
      Mcp23017GpioMap::GetTrackLedEntry(track_index,
                                        Mcp23017GpioMap::TrackLedColor::BLUE);

  Mcp23017Driver& driver = Mcp23017Driver::GetInstance();
  TrackLedPayload payload[3] = {
      {
          .address = red_entry.address,
          .led_gpio_id = Mcp23017GpioMap::GetTrackLedGpioId(
              track_index, Mcp23017GpioMap::TrackLedColor::RED),
          .led_state = track_state_led_color_map[state_id].red,
      },
      {
          .address = green_entry.address,
          .led_gpio_id = Mcp23017GpioMap::GetTrackLedGpioId(
              track_index, Mcp23017GpioMap::TrackLedColor::GREEN),
          .led_state = track_state_led_color_map[state_id].green,
      },
      {
          .address = blue_entry.address,
          .led_gpio_id = Mcp23017GpioMap::GetTrackLedGpioId(
              track_index, Mcp23017GpioMap::TrackLedColor::BLUE),
          .led_state = track_state_led_color_map[state_id].blue,
      },
  };
  if (driver.UpdateTrackLedState(payload) != Mcp23017Status::OK) {
    return TASK_STATUS_ERROR;
  }
  return TASK_STATUS_OK;
}
