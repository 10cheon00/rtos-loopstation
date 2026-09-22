#ifndef GMG12864_LCD_HPP
#define GMG12864_LCD_HPP

#include <cstdbool>
#include <cstdint>

#include "U8g2lib.h"
#include "stm32h7xx.h"

namespace Gmg12864 {

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define GMG12864_TIMEOUT_MS 1000

enum class Gmg12864Status {
  OK = 0,
  ERROR,
  NOT_INITED,
};

struct SpiParams {
  SPI_HandleTypeDef* hspi;
  uint16_t CS_Pin;
  uint16_t RST_Pin;
  uint16_t DC_Pin;
  GPIO_TypeDef* CS_Port;
  GPIO_TypeDef* RST_Port;
  GPIO_TypeDef* DC_Port;
};

struct InitParams {
  SpiParams spi_params;
  const u8g2_cb_t* rotation;
};

uint8_t u8g2MessageCallback(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int,
                            void* arg_ptr);

uint8_t u8g2GpioAndDelayCallback(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int,
                                 void* arg_ptr);

class Driver : public U8G2 {
 private:
  Driver() : U8G2() {}
  ~Driver() {}
  Driver(const Driver& ref) = delete;
  Driver& operator=(const Driver& ref) = delete;

 public:
  static Driver& GetInstance() {
    static Driver driver;
    return driver;
  }

 public:
  Gmg12864Status Initialize(InitParams* params) {
    status = Gmg12864Status::NOT_INITED;
    this->spi_params = params->spi_params;

    u8g2_Setup_st7565_erc12864_alt_f(&this->u8g2, params->rotation,
                                     u8g2MessageCallback,
                                     u8g2GpioAndDelayCallback);
    begin();
    setContrast(80);
    status = Gmg12864Status::OK;
    return status;
  }

  SpiParams& GetSpiParams() { return spi_params; }

  uint16_t GetGlyphWidth(const uint16_t encoding) {
    return u8g2_GetGlyphWidth(&this->u8g2, encoding);
  }

 private:
  Gmg12864Status status;
  SpiParams spi_params;

  bool IsValidInitParams(const InitParams* params) {
    return (params != 0) &&
           (params->spi_params.hspi != NULL && params->spi_params.CS_Pin != 0 &&
            params->spi_params.CS_Port != NULL &&
            params->spi_params.RST_Pin != 0 &&
            params->spi_params.RST_Port != NULL &&
            params->spi_params.DC_Pin != 0 &&
            params->spi_params.DC_Port != NULL);
  }
};

}  // namespace Gmg12864

#endif
