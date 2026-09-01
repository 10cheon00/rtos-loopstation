#include "mutex_guard.hpp"

MutexGuard::MutexGuard(osMutexId_t mutex, uint32_t timeout_tick)
    : mutex(mutex), timeout_tick(timeout_tick),
      acquired(osMutexAcquire(this->mutex, this->timeout_tick) == osOK)
{}

MutexGuard::~MutexGuard()
{
    if (acquired) {
        osMutexRelease(this->mutex);
    }
}
