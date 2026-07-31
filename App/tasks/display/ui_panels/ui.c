#include "ui.h"

void UI_DrawArrowLeft4x5(u8g2_t *u8g2, uint8_t x, uint8_t y)
{
    u8g2_DrawVLine(u8g2, x, y - 3, 1);
    u8g2_DrawVLine(u8g2, x + 1, y - 4, 3);
    u8g2_DrawVLine(u8g2, x + 2, y - 4, 3);
    u8g2_DrawVLine(u8g2, x + 3, y - 5, 5);
}

void UI_DrawArrowRight4x5(u8g2_t *u8g2, uint8_t x, uint8_t y)
{
    u8g2_DrawVLine(u8g2, x, y - 5, 5);
    u8g2_DrawVLine(u8g2, x + 1, y - 4, 3);
    u8g2_DrawVLine(u8g2, x + 2, y - 4, 3);
    u8g2_DrawVLine(u8g2, x + 3, y - 3, 1);
}

void UI_ConvertNumberToString(int32_t number, char *string, uint8_t string_length)
{
    uint8_t i = 0, j = 0, is_negative = number < 0 ? 1 : 0;
    char c;
    while (number != 0) {
        string[i] = number % 10 + '0';
        number /= 10;
        i++;
    }
    if (is_negative) {
        string[i] = '-';
        i++;
    }
    while (j < i - 1) {
        c = string[j];
        string[j] = string[i - 1];
        string[i - 1] = c;
        j++;
        i--;
    }
}
