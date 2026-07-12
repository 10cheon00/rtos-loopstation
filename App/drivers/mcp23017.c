#include "mcp23017.h"

Mcp23017Status Mcp23017_Init(uint8_t address)
{
    (void)address;
    return MCP23017_STATUS_OK;
}

Mcp23017Status Mcp23017_ReadRegister(uint8_t address, uint8_t reg, uint8_t *value)
{
    (void)address;
    (void)reg;

    if (value == 0) {
        return MCP23017_STATUS_ERROR;
    }

    *value = 0U;
    return MCP23017_STATUS_OK;
}

Mcp23017Status Mcp23017_WriteRegister(uint8_t address, uint8_t reg, uint8_t value)
{
    (void)address;
    (void)reg;
    (void)value;
    return MCP23017_STATUS_OK;
}
