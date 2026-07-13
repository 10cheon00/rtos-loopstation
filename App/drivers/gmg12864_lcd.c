#include "gmg12864_lcd.h"

static int Gmg12864LcdStatus_IsValidInitParams(const Gmg12864Lcd_InitParams *params)
{
    return (params != 0) && (params->hspi != NULL && params->CS_Pin != 0 &&
                             params->CS_Port != NULL && params->RST_Pin != 0 &&
                             params->RST_Port != NULL && params->DC_Pin != 0 &&
                             params->DC_Port != NULL);
}

static uint8_t Gmg12864Lcd_u8g2MessageCallback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                                               void *arg_ptr);
static uint8_t Gmg12864Lcd_u8g2GpioAndDelayCallback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                                                    void *arg_ptr);

Gmg12864LcdStatus Gmg12864Lcd_Init(u8g2_t *u8g2, Gmg12864Lcd_InitParams *params)
{
    if (!(u8g2 != NULL && Gmg12864LcdStatus_IsValidInitParams(params))) {
        return GMG12864_LCD_STATUS_ERROR;
    }
    u8x8_SetUserPtr(&u8g2->u8x8, params);

    u8g2_Setup_st7565_erc12864_alt_f(u8g2, NULL, Gmg12864Lcd_u8g2MessageCallback,
                                     Gmg12864Lcd_u8g2GpioAndDelayCallback);
    return GMG12864_LCD_STATUS_OK;
}

static uint8_t Gmg12864Lcd_u8g2MessageCallback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                                               void *arg_ptr)
{
    switch (msg) {
    case U8X8_MSG_BYTE_INIT:
        break;
    case U8X8_MSG_BYTE_SEND:
        break;
    case U8X8_MSG_BYTE_SET_DC:
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        break;
    }
    return 0;
}

static uint8_t Gmg12864Lcd_u8g2GpioAndDelayCallback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                                                    void *arg_ptr)
{
    switch (msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        break;
    case U8X8_MSG_DELAY_MILLI:
        break;
    case U8X8_MSG_DELAY_10MICRO:
        break;
    case U8X8_MSG_DELAY_100NANO:
        break;
    case U8X8_MSG_GPIO_RESET:
        break;
    case U8X8_MSG_GPIO_CS:
        break;
    case U8X8_MSG_GPIO_DC:
        break;
    }
    return 0;
}
