#include "state_machine.h"

StateMachineStatus StateMachine_DoTransition(StateMachine *state_machine,
                                             StateTransition *state_transition)
{
    state_machine->current_state = state_transition->to;
    
    return STATE_MACHINE_STATUS_OK;
}
