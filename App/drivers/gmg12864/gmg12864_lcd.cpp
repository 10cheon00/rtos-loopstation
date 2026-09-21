#include "gmg12864_lcd.hpp"

namespace Gmg12864 {

uint8_t u8g2MessageCallback(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int,
                            void* arg_ptr) {
  Driver& driver = Driver::GetInstance();
  switch (msg) {
    case U8X8_MSG_BYTE_INIT:
      u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
      break;
    case U8X8_MSG_BYTE_SEND:
      HAL_SPI_Transmit(driver.GetSpiParams().hspi, (const uint8_t*)arg_ptr,
                       arg_int, GMG12864_TIMEOUT_MS);
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

uint8_t u8g2GpioAndDelayCallback(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int,
                                 void* arg_ptr) {
  uint32_t i = 0;
  Driver& driver = Driver::GetInstance();
  SpiParams& params = driver.GetSpiParams();
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
      HAL_GPIO_WritePin(params.RST_Port, params.RST_Pin,
                        arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;
    case U8X8_MSG_GPIO_CS:
      HAL_GPIO_WritePin(params.CS_Port, params.CS_Pin,
                        arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;
    case U8X8_MSG_GPIO_DC:
      HAL_GPIO_WritePin(params.DC_Port, params.DC_Pin,
                        arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;
    default:
      return 0;
  }
  return 1;
}

}  // namespace Gmg12864