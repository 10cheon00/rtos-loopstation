#ifndef MCP23017_H
#define MCP23017_H

#include <stdint.h>

#include "stm32h7xx.h"
#include "cmsis_os2.h"

#include "mcp23017_types.hpp"
#include "mcp23017_gpio_id.hpp"

enum class Mcp23017LedState : uint8_t {
    OFF = 0x0,
    ON = 0xFF,
};

struct Mcp23017PortInterruptSnapshot {
    Mcp23017PinMask pin_mask;
    Mcp23017PinStates captured_pin_states;
};

struct Mcp23017InterruptSnapshot {
    Mcp23017PortInterruptSnapshot port_a;
    Mcp23017PortInterruptSnapshot port_b;
};

struct Mcp23017AddressInterruptPinMap {
    Mcp23017Address address;
    Mcp23017InterruptPin gpio_interrupt_pin;
};

enum class Mcp23017InitStatus {
    NOT_INITED = 0,
    ON_INIT,
    INITED,
};

struct TrackLedPayload {
    Mcp23017Address address;
    Mcp23017GpioId led_gpio_id;
    Mcp23017LedState led_state;
};

class Mcp23017Driver {
  private:
    Mcp23017Driver() {}
    ~Mcp23017Driver() {}
    Mcp23017Driver(const Mcp23017Driver &ref) = delete;
    Mcp23017Driver &operator=(const Mcp23017Driver &ref) = delete;

  public:
    static Mcp23017Driver &GetInstance()
    {
        static Mcp23017Driver driver;
        return driver;
    }

    Mcp23017Status Initialize(I2C_HandleTypeDef *hi2c, osSemaphoreId_t i2c_mutex);
    Mcp23017Status GetInterruptSnapshot(Mcp23017Address address,
                                        Mcp23017InterruptSnapshot *snapshot);
    Mcp23017Status GetMcp23017AddressFromInterruptPin(Mcp23017InterruptPin gpio_interrupt_pin,
                                                      Mcp23017Address *address);
    Mcp23017Status UpdateLedState(Mcp23017Address address, Mcp23017GpioId led_gpio_id,
                                  Mcp23017LedState led_state);
    Mcp23017Status UpdateTrackLedState(TrackLedPayload payload[3]);

  private:
    I2C_HandleTypeDef *hi2c;
    Mcp23017InitStatus init_status;
    osSemaphoreId_t i2c_mutex;

    Mcp23017Status InternalInitialize(Mcp23017Address address);
    Mcp23017Status ReadRegister(Mcp23017Address address, uint8_t reg, uint8_t *value);
    Mcp23017Status WriteRegister(Mcp23017Address address, uint8_t reg, uint8_t value);
    Mcp23017Status UpdateLedStateLocked(Mcp23017Address address, Mcp23017GpioId led_gpio_id,
                                  Mcp23017LedState led_state);
};

// TODO: 싱글톤으로 모든 드라이버를 제공하고, 필요하면 입출력 드라이버 나누기
// class Mcp23017InputDriver {
//   public:
//     Mcp23017InputDriver(I2C_HandleTypeDef *hi2c);

//     // ISR이 전달한 GPIO 핀으로 실제 address를 알아내 반환하는 함수
//     Mcp23017Status GetMcp23017AddressFromInterruptPin(Mcp23017InterruptPin gpio_interrupt_pin,
//                                                       Mcp23017Address *address);
//     Mcp23017Status GetInterruptSnapshot(Mcp23017Address address, Mcp23017InterruptSnapshot
//     *state);

//   private:
//     I2C_HandleTypeDef *hi2c;
// };

// class Mcp23017OutputDriver {
//   public:
//     Mcp23017OutputDriver(I2C_HandleTypeDef *hi2c);

//     Mcp23017Status UpdateLedState(Mcp23017Address address, Mcp23017GpioId led_gpio_id,
//                                   Mcp23017LedState led_state);

//   private:
//     I2C_HandleTypeDef *hi2c;
// };

#endif
