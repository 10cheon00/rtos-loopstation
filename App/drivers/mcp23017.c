#include "mcp23017.h"

#include "mcp23017_gpio_table.h"
#include "utils.h"

#define IODIRA 0x0
#define IODIRB 0x1
#define GPINTENA 0x4
#define GPINTENB 0x5
#define DEFVALA 0x6
#define DEFVALB 0x7
#define INTCONA 0x8
#define INTCONB 0x9
#define IOCONA 0xA
#define IOCONB 0xB
#define GPPUA 0xC
#define GPPUB 0xD
#define _GPIOA 0x12
#define _GPIOB 0x13
#define OLATA 0x14
#define OLATB 0x15
#define INTCON_AGAINST_PREVIOUS_PIN_VALUE 0

#define IOCON_BANK_0 0x00
#define IOCON_MIRROR_1 0x40
#define IOCON_SEQOP_DISABLE 0x20
#define IOCON_DISSLW_DISABLE 0x10
#define IOCON_HAEN_DISABLE 0x00
#define IOCON_ODR_DISABLE 0x00
#define IOCON_INTPOL_ACTIVE_HIGH 0x02
#define IOCON_SETTING                                                                              \
    IOCON_BANK_0 | IOCON_MIRROR_1 | IOCON_SEQOP_DISABLE | IOCON_DISSLW_DISABLE |                   \
        IOCON_HAEN_DISABLE | IOCON_ODR_DISABLE | IOCON_INTPOL_ACTIVE_HIGH

#define MCP23017_TIMEOUT_MS 500

static const Mcp23017AddressInterruptPinMap interrupt_pin_map[] = {
    {.address = MCP23017_ADDRESS_0B100, .gpio_interrupt_pin = GPIO_PIN_0},
    {.address = MCP23017_ADDRESS_0B101, .gpio_interrupt_pin = GPIO_PIN_1},
};
static const size_t interrupt_pin_map_count = ARRAY_COUNT(interrupt_pin_map);

static Mcp23017Status Mcp23017_IsValidInitParams(Mcp23017InitParams *params)
{
    if (params != NULL && (params->hi2c != NULL)) {
        return MCP23017_STATUS_OK;
    }
    return MCP23017_STATUS_ERROR;
}

Mcp23017Status Mcp23017_Init(Mcp23017InitParams *params)
{
    if (Mcp23017_IsValidInitParams(params) != MCP23017_STATUS_OK) {
        return MCP23017_STATUS_ERROR;
    }
    /*
  1. IODIRA/IODIRB: 버튼 핀을 입력으로 설정
  2. GPPUA/GPPUB: 필요한 경우 버튼 입력 내부 pull-up 활성화
  3. IOCON: INTA 출력 방식, 극성, INTA/INTB 미러링 설정
  4. INTCONA/INTCONB: 인터럽트 발생 기준 설정
  5. DEFVALA/DEFVALB: 기본값 비교 방식을 쓸 때 기준값 설정
  6. GPIOA/GPIOB를 읽어 이전 상태와 pending interrupt 초기화
  7. GPINTENA/GPINTENB: 버튼 핀의 interrupt-on-change 활성화
    */
    I2C_HandleTypeDef *hi2c = params->hi2c;
    Mcp23017Status status;
    Mcp23017Address address;
    uint8_t dummy;
    Mcp23017GpioPinMask port_a_input_pin_mask, port_b_input_pin_mask;

    for (size_t i = 0; i < interrupt_pin_map_count; i++) {
        address = interrupt_pin_map[i].address;
        port_a_input_pin_mask = Mcp23017GpioMap_GetInputPinMask(address, MCP23017_GPIO_PORT_A);
        port_b_input_pin_mask = Mcp23017GpioMap_GetInputPinMask(address, MCP23017_GPIO_PORT_B);

        if (HAL_I2C_IsDeviceReady(hi2c, address << 1, 10, MCP23017_TIMEOUT_MS) != HAL_OK) {
            return MCP23017_STATUS_ERROR;
        };
        status = Mcp23017_WriteRegister(hi2c, address, GPINTENA, 0);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_WriteRegister(hi2c, address, GPINTENB, 0);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_WriteRegister(hi2c, address, IOCONA, IOCON_SETTING);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_WriteRegister(hi2c, address, IOCONB, IOCON_SETTING);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_WriteRegister(hi2c, address, IODIRA, port_a_input_pin_mask);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_WriteRegister(hi2c, address, IODIRB, port_b_input_pin_mask);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_WriteRegister(hi2c, address, GPPUA, port_a_input_pin_mask);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_WriteRegister(hi2c, address, GPPUB, port_b_input_pin_mask);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_WriteRegister(hi2c, address, INTCONA, INTCON_AGAINST_PREVIOUS_PIN_VALUE);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_WriteRegister(hi2c, address, INTCONB, INTCON_AGAINST_PREVIOUS_PIN_VALUE);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_ReadRegister(hi2c, address, _GPIOA, &dummy);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_ReadRegister(hi2c, address, _GPIOB, &dummy);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_WriteRegister(hi2c, address, GPINTENA, port_a_input_pin_mask);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
        status = Mcp23017_WriteRegister(hi2c, address, GPINTENB, port_b_input_pin_mask);
        if (status == MCP23017_STATUS_ERROR) {
            return status;
        }
    }

    return MCP23017_STATUS_OK;
}

Mcp23017Status Mcp23017_ReadRegister(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t reg,
                                     uint8_t *value)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, address << 1, reg, I2C_MEMADD_SIZE_8BIT,
                                                value, 1, MCP23017_TIMEOUT_MS);
    if (status == HAL_OK) {
        return MCP23017_STATUS_OK;
    }

    return MCP23017_STATUS_ERROR;
}

Mcp23017Status Mcp23017_WriteRegister(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t reg,
                                      uint8_t value)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hi2c, address << 1, reg, I2C_MEMADD_SIZE_8BIT,
                                                 &value, 1, MCP23017_TIMEOUT_MS);
    if (status == HAL_OK) {
        return MCP23017_STATUS_OK;
    }
    return MCP23017_STATUS_ERROR;
}

Mcp23017Status Mcp23017_UpdateOutputPinState(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t port,
                                             uint8_t pin_register_mask, uint8_t pin_state)
{
    uint8_t value;
    uint8_t reg = port == MCP23017_GPIO_PORT_A ? OLATA : OLATB;
    Mcp23017Status status = Mcp23017_ReadRegister(hi2c, address, reg, &value);
    if (status != MCP23017_STATUS_OK) {
        return status;
    }

    value = (value & (uint8_t)~pin_register_mask) | pin_state;

    return Mcp23017_WriteRegister(hi2c, address, reg, value);
}

Mcp23017Status Mcp23017_GetMcp23017AddressFromInterruptPin(Mcp23017GpioInterruptPin gpio_interrupt_pin, uint8_t *address)
{
    for (size_t i = 0; i < interrupt_pin_map_count; i++) {
        if (interrupt_pin_map[i].gpio_interrupt_pin == gpio_interrupt_pin) {
            *address = interrupt_pin_map[i].address;
            return MCP23017_STATUS_OK;
        }
    }
    return MCP23017_STATUS_ERROR;
}
