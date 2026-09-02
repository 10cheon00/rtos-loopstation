#ifndef MCP23017_H
#define MCP23017_H

#include <stdint.h>

#include "cmsis_os2.h"
#include "mcp23017_gpio_id.hpp"
#include "mcp23017_types.hpp"
#include "stm32h7xx.h"

namespace Mcp23017 {

enum class LedState : uint8_t {
  OFF = 0x0,
  ON = 0xFF,
};

struct PortInterruptSnapshot {
  PinMask pin_mask;
  PinStates captured_pin_states;
};

struct InterruptSnapshot {
  PortInterruptSnapshot port_a;
  PortInterruptSnapshot port_b;
};

struct AddressInterruptPinMap {
  Address address;
  InterruptPin gpio_interrupt_pin;
};

enum class InitStatus {
  NOT_INITED = 0,
  ON_INIT,
  INITED,
};

struct TrackLedPayload {
  Address address;
  GpioId led_gpio_id;
  LedState led_state;
};

class Driver {
 private:
  Driver() {}
  ~Driver() {}
  Driver(const Driver& ref) = delete;
  Driver& operator=(const Driver& ref) = delete;

 public:
  static Driver& GetInstance() {
    static Driver driver;
    return driver;
  }

  Status Initialize(I2C_HandleTypeDef* hi2c,
                              osSemaphoreId_t i2c_mutex);
  Status GetInterruptSnapshot(Address address,
                                        InterruptSnapshot* snapshot);
  Status GetMcp23017AddressFromInterruptPin(
      InterruptPin gpio_interrupt_pin, Address* address);
  Status UpdateLedState(Address address,
                                  GpioId led_gpio_id,
                                  LedState led_state);
  Status UpdateTrackLedState(TrackLedPayload payload[3]);

 private:
  I2C_HandleTypeDef* hi2c;
  InitStatus init_status;
  osSemaphoreId_t i2c_mutex;

  Status InternalInitialize(Address address);
  Status ReadRegister(Address address, uint8_t reg,
                                uint8_t* value);
  Status WriteRegister(Address address, uint8_t reg,
                                 uint8_t value);
  Status UpdateLedStateLocked(Address address,
                                        GpioId led_gpio_id,
                                        LedState led_state);
};

}  // namespace Mcp23017

#endif
