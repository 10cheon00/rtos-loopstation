#include "state_machine.h"

StateMachineStatus StateMachine_DoTransition(StateMachine *state_machine,
                                             StateTransition *state_transition)
{
    state_machine->current_state = (State*)state_transition->to;
    state_machine->current_state->on_enter();
    
    return STATE_MACHINE_STATUS_OK;
}
