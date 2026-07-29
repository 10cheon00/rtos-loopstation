#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "state.h"

typedef enum {
    STATE_MACHINE_STATUS_OK = 0,
    STATE_MACHINE_STATUS_ERROR,
} StateMachineStatus;

typedef struct {
    State *current_state;
    void *context;
} StateMachine;

typedef struct {
    State *to;
    StateEvent *cause_event;
} StateTransition;

StateMachineStatus StateMachine_Init(StateMachine *state_machine, State* init_state, void* context);

StateMachineStatus StateMachine_DoTransition(StateMachine *state_machine,
                                             StateTransition *state_transition);
/**
 * 이 구조로 보면, 상태 머신 구조체의 current_state를 바꾸기 위해선
 *  StateMachine_DoTransition을 써야하고, 그 인자에는 이미 전이 절차가 
 *  명시된 StateTransition을 전달하도록 되어 있다.
 * 전이 과정에서 전달되어야 하는 데이터들은 on_enter에 넣는다.
 */
#endif
