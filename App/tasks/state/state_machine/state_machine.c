#include "state_machine.h"

StateMachineStatus StateMachine_Init(StateMachine *state_machine, State *init_state, void *context)
{
    StateFunctionStatus status;

    state_machine->context = context;
    state_machine->current_state = init_state;

    status = state_machine->current_state->on_enter(state_machine->context);

    if (status != STATE_FUNCTION_STATUS_OK) {
        return STATE_MACHINE_STATUS_ERROR;
    }
    return STATE_MACHINE_STATUS_OK;
}

StateMachineStatus StateMachine_DoTransition(StateMachine *state_machine,
                                             StateTransition *state_transition)
{
    state_machine->current_state = state_transition->to;

    return STATE_MACHINE_STATUS_OK;
}
