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

#ifdef __cplusplus
}
#endif

#endif
