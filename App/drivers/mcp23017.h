#ifndef MCP23017_H
#define MCP23017_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx.h"

#define MCP23017_CONTROL_REGISTER_INTFA 0xE
#define MCP23017_CONTROL_REGISTER_INTFB 0xF
#define MCP23017_CONTROL_REGISTER_INTCAPA 0x10
#define MCP23017_CONTROL_REGISTER_INTCAPB 0x11

typedef uint8_t Mcp23017Address;

#define MCP23017_ADDRESS_0B100 (0x20 | 0b100)
#define MCP23017_ADDRESS_0B101 (0x20 | 0b101)

typedef uint8_t Mcp23017GpioId;

enum {
    MCP23017_GPIO_ID_NONE = 0,
    MCP23017_GPIO_ID_LED_IFX_A,
    MCP23017_GPIO_ID_LED_TFX_A,
    MCP23017_GPIO_ID_BUTTON_LEFT,
    MCP23017_GPIO_ID_BUTTON_RIGHT,
    MCP23017_GPIO_ID_BUTTON_ENTER,
    MCP23017_GPIO_ID_BUTTON_EXIT,
    MCP23017_GPIO_ID_BUTTON_ENCODER_A_PUSH,
    MCP23017_GPIO_ID_BUTTON_ENCODER_B_PUSH,
    MCP23017_GPIO_ID_BUTTON_ENCODER_C_PUSH,
    MCP23017_GPIO_ID_BUTTON_ENCODER_D_PUSH,
    MCP23017_GPIO_ID_BUTTON_IFX_A_TOGGLE,
    MCP23017_GPIO_ID_BUTTON_TFX_A_TOGGLE,
    MCP23017_GPIO_ID_BUTTON_TRACK_1_EDIT,
    MCP23017_GPIO_ID_BUTTON_TRACK_1_PLAY_RECORD,
    MCP23017_GPIO_ID_BUTTON_TRACK_1_STOP,
    MCP23017_GPIO_ID_BUTTON_TRACK_2_EDIT,
    MCP23017_GPIO_ID_BUTTON_TRACK_2_PLAY_RECORD,
    MCP23017_GPIO_ID_BUTTON_TRACK_2_STOP,
    MCP23017_GPIO_ID_BUTTON_TRACK_3_EDIT,
    MCP23017_GPIO_ID_BUTTON_TRACK_3_PLAY_RECORD,
    MCP23017_GPIO_ID_BUTTON_TRACK_3_STOP,
    MCP23017_GPIO_ID_BUTTON_TRACK_4_EDIT,
    MCP23017_GPIO_ID_BUTTON_TRACK_4_PLAY_RECORD,
    MCP23017_GPIO_ID_BUTTON_TRACK_4_STOP,
    MCP23017_GPIO_ID_BUTTON_TRACK_5_EDIT,
    MCP23017_GPIO_ID_BUTTON_TRACK_5_PLAY_RECORD,
    MCP23017_GPIO_ID_BUTTON_TRACK_5_STOP,
};

typedef uint8_t Mcp23017GpioPort;

#define MCP23017_GPIO_PORT_A 0x0
#define MCP23017_GPIO_PORT_B 0x1

typedef enum {
    MCP23017_STATUS_OK = 0,
    MCP23017_STATUS_ERROR
} Mcp23017Status;

typedef struct {
    const uint8_t gpio_pin;
    const uint8_t address;
    const uint16_t pin_status;
} Mcp23017DeviceConfig;

typedef struct {
    I2C_HandleTypeDef *hi2c;
    const Mcp23017DeviceConfig *device_configs;
    const uint8_t device_config_count;
} Mcp23017InitParams;

Mcp23017Status Mcp23017_Init(Mcp23017InitParams *params);
Mcp23017Status Mcp23017_ReadRegister(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t reg,
                                     uint8_t *value);
Mcp23017Status Mcp23017_WriteRegister(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t reg,
                                      uint8_t value);
Mcp23017Status Mcp23017_UpdateOutputPinState(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t port,
                                             uint8_t pin_register_mask, uint8_t pin_state);

#ifdef __cplusplus
}
#endif

#endif
