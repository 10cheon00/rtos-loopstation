#ifndef UI_H
#define UI_H

#include "gmg12864_lcd.h"

typedef enum {
    UI_DRAWING_STATUS_OK = 0,
    UI_DRAWING_STATUS_ERROR
} UI_DRAWING_STATUS;

UI_DRAWING_STATUS Ui_DrawHomePanel(u8g2_t* u8g2);
UI_DRAWING_STATUS Ui_DrawSettingPanel(u8g2_t* u8g2);

#endif
