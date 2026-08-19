#ifndef UI_PANEL_ID_H 
#define UI_PANEL_ID_H

#include "id.h"
#include "state_id.h"

typedef StateId UiPanelId;

enum {
    UI_PANEL_ID_NONE = ID_NONE,
    UI_PANEL_ID_HOME,
    UI_PANEL_ID_SETTING,
    UI_PANEL_ID_SYSTEM_SETTING,
    UI_PANEL_ID_COUNT // 이 enum은 UiPanelId 개수를 표시하는 용도임
};

#endif