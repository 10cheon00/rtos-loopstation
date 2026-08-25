#ifndef UI_STATE_ID_H 
#define UI_STATE_ID_H

#include "id.h"
#include "state_id.h"

typedef StateId UiStateId;

enum {
    UI_STATE_ID_NONE = ID_NONE,
    UI_STATE_ID_HOME,
    UI_STATE_ID_SETTING,
    UI_STATE_ID_SYSTEM_SETTING,
    UI_STATE_ID_COUNT // 이 enum은 UiStateId 개수를 표시하는 용도임
};

#endif
