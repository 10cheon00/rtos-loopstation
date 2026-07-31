#include "debug_fault.h"

#include "stm32h7xx_hal.h"

volatile DebugFaultSnapshot g_debug_fault_snapshot;

static void DebugFault_BreakIfDebuggerAttached(void)
{
    if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0U) {
        __BKPT(0);
    }
}

static void DebugFault_Freeze(void)
{
    DebugFault_BreakIfDebuggerAttached();

    for (;;) {
    }
}

static void DebugFault_CaptureSystemRegisters(DebugFaultReason reason, uint32_t exc_return)
{
    g_debug_fault_snapshot.reason = (uint32_t)reason;
    g_debug_fault_snapshot.exc_return = exc_return;
    g_debug_fault_snapshot.ipsr = __get_IPSR();
    g_debug_fault_snapshot.cfsr = SCB->CFSR;
    g_debug_fault_snapshot.hfsr = SCB->HFSR;
    g_debug_fault_snapshot.dfsr = SCB->DFSR;
    g_debug_fault_snapshot.afsr = SCB->AFSR;
    g_debug_fault_snapshot.bfar = SCB->BFAR;
    g_debug_fault_snapshot.mmfar = SCB->MMFAR;
}

void DebugFault_CaptureException(DebugFaultReason reason, uint32_t *stack_pointer,
                                 uint32_t exc_return)
{
    DebugFault_CaptureSystemRegisters(reason, exc_return);

    if (stack_pointer != 0) {
        g_debug_fault_snapshot.stacked_r0 = stack_pointer[0];
        g_debug_fault_snapshot.stacked_r1 = stack_pointer[1];
        g_debug_fault_snapshot.stacked_r2 = stack_pointer[2];
        g_debug_fault_snapshot.stacked_r3 = stack_pointer[3];
        g_debug_fault_snapshot.stacked_r12 = stack_pointer[4];
        g_debug_fault_snapshot.stacked_lr = stack_pointer[5];
        g_debug_fault_snapshot.stacked_pc = stack_pointer[6];
        g_debug_fault_snapshot.stacked_xpsr = stack_pointer[7];
    }

    DebugFault_Freeze();
}

void DebugFault_CaptureSoftwareError(DebugFaultReason reason)
{
    DebugFault_CaptureSystemRegisters(reason, 0U);

    g_debug_fault_snapshot.stacked_lr = (uint32_t)__builtin_return_address(0);
    g_debug_fault_snapshot.stacked_pc = g_debug_fault_snapshot.stacked_lr;

    DebugFault_Freeze();
}
