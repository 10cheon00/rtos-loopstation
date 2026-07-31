#ifndef DEBUG_FAULT_H
#define DEBUG_FAULT_H

#include <stdint.h>

typedef enum {
    DEBUG_FAULT_REASON_NMI = 1,
    DEBUG_FAULT_REASON_HARDFAULT,
    DEBUG_FAULT_REASON_MEMMANAGE,
    DEBUG_FAULT_REASON_BUSFAULT,
    DEBUG_FAULT_REASON_USAGEFAULT,
    DEBUG_FAULT_REASON_ERROR_HANDLER,
} DebugFaultReason;

typedef struct {
    uint32_t reason;
    uint32_t stacked_r0;
    uint32_t stacked_r1;
    uint32_t stacked_r2;
    uint32_t stacked_r3;
    uint32_t stacked_r12;
    uint32_t stacked_lr;
    uint32_t stacked_pc;
    uint32_t stacked_xpsr;
    uint32_t exc_return;
    uint32_t ipsr;
    uint32_t cfsr;
    uint32_t hfsr;
    uint32_t dfsr;
    uint32_t afsr;
    uint32_t bfar;
    uint32_t mmfar;
} DebugFaultSnapshot;

extern volatile DebugFaultSnapshot g_debug_fault_snapshot;

void DebugFault_CaptureException(DebugFaultReason reason, uint32_t *stack_pointer,
                                 uint32_t exc_return);
void DebugFault_CaptureSoftwareError(DebugFaultReason reason);

#endif
