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
    uint8_t i = string_length - 1, j, number_length = 0, is_negative = number < 0 ? 1 : 0;

    string[i] = '\0';
    i--;
    string[i] = '0';
    while (i >= 0) {
        if (number == 0) {
            if (is_negative) {
                string[i] = '-';
            }
            break;
        }
        string[i] = number % 10 + '0';
        i--;
        number_length++;
        number /= 10;
    }
}
