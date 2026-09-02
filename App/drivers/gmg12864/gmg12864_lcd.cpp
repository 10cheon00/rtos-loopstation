#include "gmg12864_lcd.hpp"

namespace Gmg12864 {
#define GMG12864_TIMEOUT_MS 1000

static int Gmg12864LcdStatus_IsValidInitParams(const InitParams* params) {
  return (params != 0) && (params->hspi != NULL && params->CS_Pin != 0 &&
                           params->CS_Port != NULL && params->RST_Pin != 0 &&
                           params->RST_Port != NULL && params->DC_Pin != 0 &&
                           params->DC_Port != NULL);
}

static uint8_t Gmg12864Lcd_u8g2MessageCallback(u8x8_t* u8x8, uint8_t msg,
                                               uint8_t arg_int, void* arg_ptr) {
  InitParams* params = (InitParams*)u8x8->user_ptr;
  SPI_HandleTypeDef* hspi = params->hspi;

  switch (msg) {
    case U8X8_MSG_BYTE_INIT:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;
    case U8X8_MSG_BYTE_SEND:
      HAL_SPI_Transmit(hspi, (const uint8_t*)arg_ptr, arg_int,
                       GMG12864_TIMEOUT_MS);
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8x8, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;
    default:
      return 0;
  }
  return 1;
}

static uint8_t Gmg12864Lcd_u8g2GpioAndDelayCallback(u8x8_t* u8x8, uint8_t msg,
                                                    uint8_t arg_int,
                                                    void* arg_ptr) {
  InitParams* params = (InitParams*)u8x8->user_ptr;
  uint32_t i = 0;
  switch (msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      break;
    case U8X8_MSG_DELAY_MILLI:
      HAL_Delay(arg_int);
      break;
    case U8X8_MSG_DELAY_10MICRO:
      while (i < (uint32_t)arg_int * 1600U) {
        i++;
      }
      break;
    case U8X8_MSG_DELAY_100NANO:
      while (i < (uint32_t)arg_int * 16U) {
        i++;
      }
      break;
    case U8X8_MSG_DELAY_NANO:
      break;
    case U8X8_MSG_GPIO_RESET:
      HAL_GPIO_WritePin(params->RST_Port, params->RST_Pin,
                        arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;
    case U8X8_MSG_GPIO_CS:
      HAL_GPIO_WritePin(params->CS_Port, params->CS_Pin,
                        arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;
    case U8X8_MSG_GPIO_DC:
      HAL_GPIO_WritePin(params->DC_Port, params->DC_Pin,
                        arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;
    default:
      return 0;
  }
  return 1;
}

LcdStatus Init(u8g2_t* u8g2, InitParams* params) {
  if (!(u8g2 != NULL && Gmg12864LcdStatus_IsValidInitParams(params))) {
    return LcdStatus::ERROR;
  }
  u8x8_SetUserPtr(&u8g2->u8x8, params);

  u8g2_Setup_st7565_erc12864_alt_f(u8g2, U8G2_R0,
                                   Gmg12864Lcd_u8g2MessageCallback,
                                   Gmg12864Lcd_u8g2GpioAndDelayCallback);
  u8g2_InitDisplay(u8g2);
  u8g2_SetPowerSave(u8g2, 0);
  u8g2_SetContrast(u8g2, 80);

  return LcdStatus::OK;
}

}  // namespace Gmg12864
