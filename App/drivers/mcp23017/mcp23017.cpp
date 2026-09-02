#include "mcp23017.hpp"

#include "mcp23017_gpio_map.hpp"

#include "mutex_guard.hpp"
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

#define INTFA 0xE
#define INTFB 0xF
#define INTCAPA 0x10
#define INTCAPB 0x11

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

Mcp23017Status Mcp23017Driver::Initialize(I2C_HandleTypeDef *hi2c, osSemaphoreId_t i2c_mutex)
{
    MutexGuard lock{i2c_mutex, pdMS_TO_TICKS(500UL)};
    if (!lock.is_acquired()) {
        this->init_status = Mcp23017InitStatus::NOT_INITED;
        return Mcp23017Status::BUSY;
    }

    if (init_status == Mcp23017InitStatus::INITED) {
        return Mcp23017Status::OK;
    }
    if (hi2c == nullptr || i2c_mutex == nullptr) {
        return Mcp23017Status::ERROR;
    }
    this->init_status = Mcp23017InitStatus::ON_INIT;
    this->hi2c = hi2c;
    this->i2c_mutex = i2c_mutex;

    for (size_t i = 0; i < interrupt_pin_map_count; i++) {
        Mcp23017Address address = interrupt_pin_map[i].address;

        if (this->InternalInitialize(address) != Mcp23017Status::OK) {
            init_status = Mcp23017InitStatus::NOT_INITED;
            return Mcp23017Status::ERROR;
        }
        // TODO:
        // Mcp23017 모듈 일부가 초기화되지 않을 수 있으므로 로그를 남기기
    }

    init_status = Mcp23017InitStatus::INITED;
    return Mcp23017Status::OK;
}

/**
 * 1. IODIRA/IODIRB: 버튼 핀을 입력으로 설정
 * 2. GPPUA/GPPUB: 필요한 경우 버튼 입력 내부 pull-up 활성화
 * 3. IOCON: INTA 출력 방식, 극성, INTA/INTB 미러링 설정
 * 4. INTCONA/INTCONB: 인터럽트 발생 기준 설정
 * 5. DEFVALA/DEFVALB: 기본값 비교 방식을 쓸 때 기준값 설정
 * 6. GPIOA/GPIOB를 읽어 이전 상태와 pending interrupt 초기화
 * 7. GPINTENA/GPINTENB: 버튼 핀의 interrupt-on-change 활성화
 */
Mcp23017Status Mcp23017Driver::InternalInitialize(Mcp23017Address address)
{

    Mcp23017PinMask port_a_input_pin_mask =
        Mcp23017GpioMap_GetInputPinMask(address, MCP23017_GPIO_PORT_A);
    Mcp23017PinMask port_b_input_pin_mask =
        Mcp23017GpioMap_GetInputPinMask(address, MCP23017_GPIO_PORT_B);

    if (HAL_I2C_IsDeviceReady(this->hi2c, address << 1, 10, MCP23017_TIMEOUT_MS) != HAL_OK) {
        return Mcp23017Status::ERROR;
    }

    Mcp23017Status status = WriteRegister(address, GPINTENA, 0);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    status = WriteRegister(address, GPINTENB, 0);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    status = WriteRegister(address, IOCONA, IOCON_SETTING);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    status = WriteRegister(address, IOCONB, IOCON_SETTING);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    status = WriteRegister(address, IODIRA, port_a_input_pin_mask);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    status = WriteRegister(address, IODIRB, port_b_input_pin_mask);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    status = WriteRegister(address, GPPUA, port_a_input_pin_mask);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    status = WriteRegister(address, GPPUB, port_b_input_pin_mask);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    status = WriteRegister(address, INTCONA, INTCON_AGAINST_PREVIOUS_PIN_VALUE);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    status = WriteRegister(address, INTCONB, INTCON_AGAINST_PREVIOUS_PIN_VALUE);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    uint8_t dummy;
    status = ReadRegister(address, _GPIOA, &dummy);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    status = ReadRegister(address, _GPIOB, &dummy);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    status = WriteRegister(address, GPINTENA, port_a_input_pin_mask);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    status = WriteRegister(address, GPINTENB, port_b_input_pin_mask);
    if (status == Mcp23017Status::ERROR) {
        return status;
    }
    return Mcp23017Status::OK;
}

Mcp23017Status Mcp23017Driver::GetInterruptSnapshot(Mcp23017Address address,
                                                    Mcp23017InterruptSnapshot *snapshot)
{
    if (this->init_status != Mcp23017InitStatus::INITED) {
        return Mcp23017Status::NOT_INITED;
    }

    MutexGuard lock{i2c_mutex, 500UL};
    if (!lock.is_acquired()) {
        return Mcp23017Status::BUSY;
    }

    Mcp23017Status status;
    status = this->ReadRegister(address, INTFA, &snapshot->port_a.pin_mask);
    if (status != Mcp23017Status::OK) {
        return status;
    }
    status = this->ReadRegister(address, INTFB, &snapshot->port_b.pin_mask);
    if (status != Mcp23017Status::OK) {
        return status;
    }

    if (snapshot->port_a.pin_mask != 0) {
        status = this->ReadRegister(address, INTCAPA, &snapshot->port_a.captured_pin_states);
        if (status != Mcp23017Status::OK) {
            return status;
        }
    }
    if (snapshot->port_b.pin_mask != 0) {
        status = this->ReadRegister(address, INTCAPB, &snapshot->port_b.captured_pin_states);
        if (status != Mcp23017Status::OK) {
            return status;
        }
    }

    return status;
}

Mcp23017Status Mcp23017Driver::UpdateLedState(Mcp23017Address address, Mcp23017GpioId led_gpio_id,
                                              Mcp23017LedState led_state)
{
    if (this->init_status != Mcp23017InitStatus::INITED) {
        return Mcp23017Status::NOT_INITED;
    }

    MutexGuard lock{i2c_mutex, 500UL};
    if (!lock.is_acquired()) {
        return Mcp23017Status::BUSY;
    }
    return UpdateLedStateLocked(address, led_gpio_id, led_state);
}

Mcp23017Status Mcp23017Driver::UpdateTrackLedState(TrackLedPayload payload[3])
{
    if (this->init_status != Mcp23017InitStatus::INITED) {
        return Mcp23017Status::NOT_INITED;
    }

    MutexGuard lock{i2c_mutex, 500UL};
    if (!lock.is_acquired()) {
        return Mcp23017Status::BUSY;
    }

    for (uint8_t i = 0; i < 3; i++) {
        if (UpdateLedStateLocked(payload[i].address, payload[i].led_gpio_id,
                                 payload[i].led_state) != Mcp23017Status::OK) {
            return Mcp23017Status::ERROR;
        }
    }
    return Mcp23017Status::OK;
}

Mcp23017Status
Mcp23017Driver::GetMcp23017AddressFromInterruptPin(Mcp23017InterruptPin gpio_interrupt_pin,
                                                   Mcp23017Address *address)
{
    for (size_t i = 0; i < interrupt_pin_map_count; i++) {
        if (interrupt_pin_map[i].gpio_interrupt_pin == gpio_interrupt_pin) {
            *address = interrupt_pin_map[i].address;
            return Mcp23017Status::OK;
        }
    }
    return Mcp23017Status::ERROR;
}

Mcp23017Status Mcp23017Driver::UpdateLedStateLocked(Mcp23017Address address,
                                                    Mcp23017GpioId led_gpio_id,
                                                    Mcp23017LedState led_state)
{
    Mcp23017Status status;
    ParameterPinMapEntry *entry = Mcp23017GpioMap_GetEntry(led_gpio_id);
    if (entry == NULL) {
        return Mcp23017Status::ERROR;
    }
    Mcp23017Port port = entry->port;

    uint8_t reg = port == MCP23017_GPIO_PORT_A ? OLATA : OLATB, value;
    status = this->ReadRegister(address, reg, &value);
    if (status != Mcp23017Status::OK) {
        return status;
    }
    Mcp23017PinMask pin_register_mask = (Mcp23017PinMask)0x1 << entry->pin_index;

    value = (value & (uint8_t)~pin_register_mask) | (uint8_t)led_state;

    return this->WriteRegister(address, reg, value);
}

Mcp23017Status Mcp23017Driver::ReadRegister(Mcp23017Address address, uint8_t reg, uint8_t *value)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, address << 1, reg, I2C_MEMADD_SIZE_8BIT,
                                                value, 1, MCP23017_TIMEOUT_MS);
    if (status == HAL_OK) {
        return Mcp23017Status::OK;
    }
    return Mcp23017Status::ERROR;
}

Mcp23017Status Mcp23017Driver::WriteRegister(Mcp23017Address address, uint8_t reg, uint8_t value)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hi2c, address << 1, reg, I2C_MEMADD_SIZE_8BIT,
                                                 &value, 1, MCP23017_TIMEOUT_MS);
    if (status == HAL_OK) {
        return Mcp23017Status::OK;
    }
    return Mcp23017Status::ERROR;
}
