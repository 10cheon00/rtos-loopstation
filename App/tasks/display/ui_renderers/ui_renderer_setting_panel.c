#include "ui_renderer_setting_panel.h"

#include "ui_renderer_utils.h"

UiDrawingStatus Ui_DrawSettingPanel(u8g2_t *u8g2, UiParameterRenderSlot *parameter_slots)
{
    UI_DrawPanelLayout(u8g2, "SETTING", UI_ARROW_FLAG_RIGHT);

    UI_DrawPanelMenu(u8g2, UI_PANEL_SLOT_ICON_ID_SYSTEM, "SYSTEM", UI_STATE_SLOT_INDEX_A);
    UI_DrawPanelMenu(u8g2, UI_PANEL_SLOT_ICON_ID_DEBUG, "DEBUG\nHW", UI_STATE_SLOT_INDEX_B);
    UI_DrawPanelMenu(u8g2, UI_PANEL_SLOT_ICON_ID_SYSTEM, "SYSTEM", UI_STATE_SLOT_INDEX_C);
    UI_DrawPanelMenu(u8g2, UI_PANEL_SLOT_ICON_ID_SYSTEM, "SYSTEM", UI_STATE_SLOT_INDEX_D);
    return UI_DRAWING_STATUS_OK;
}
