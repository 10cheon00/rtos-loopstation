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
    const State *to;
    const StateEvent *cause_event;
    void *context; // TODO: 모든 상태 머신에 컨텍스트 멤버가 필요한지 판단하기
} StateTransition;

StateMachineStatus StateMachine_DoTransition(StateMachine *state_machine,
                                             StateTransition *state_transition);
/**
 * 이 구조로 보면, 상태 머신 구조체의 current_state를 바꾸기 위해선
 *  StateMachine_DoTransition을 써야하고, 그 인자에는 이미 전이 절차가 
 *  명시된 StateTransition을 전달하도록 되어 있다.
 * 전이 과정에서 전달되어야 하는 데이터들은 on_enter에 넣는다.
 */
#endif
