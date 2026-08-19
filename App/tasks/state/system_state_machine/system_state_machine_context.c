#include "system_state_machine_context.h"

void SystemStateMachineContext_Init(SystemStateMachineContext *context) {
    context->logs = ConfigValidator_GetLog();
}
