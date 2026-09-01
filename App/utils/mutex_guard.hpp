#ifndef MUTEX_GUARD_HPP
#define MUTEX_GUARD_HPP

#include <cstdint>
#include <cstdbool>
#include "FreeRTOS.h"
#include "cmsis_os2.h"

class MutexGuard {
  public:
    MutexGuard(osMutexId_t mutex, TickType_t timeout_tick);
    ~MutexGuard();

    bool is_acquired() const
    {
        return this->acquired;
    };

  private:
    osMutexId_t mutex;
    TickType_t timeout_tick;
    bool acquired;
};

#endif
