#include "stdint.h"
#include "TFTMaster.h"
#include "TouchScreen.h"
#include <stdio.h>
#include "stdbool.h"

struct TSPoint p;
char buffer[30];
uint16_t x, y;

bool get_ts_lcd(uint16_t *px, uint16_t *py)
{

    getPoint(&p);

    *py = ((p.x - 800) * 240) / (3500 - 800);
    *px = 320 - ((p.y - 550) * 320) / (3650 - 550);

    if (p.z < 500)
    {
        return true;
    }
    return false;
}

void ts_lcd_init()
{
    tft_init_hw();
    tft_begin();
    tft_setRotation(3);
    tft_fillScreen(ILI9340_BLACK);
    adc_init();
    p.x = 0;
    p.y = 0;
    p.z = 0;
}

void ts_test()
{
    uint16_t newX, newY;
    bool isTouched = get_ts_lcd(&newX, &newY);

    // black screen
    tft_setCursor(20, 100);
    tft_setTextColor(ILI9340_WHITE);
    tft_setTextSize(2);

    // erase old text
    tft_setTextColor(ILI9340_BLACK);
    tft_writeString(buffer);

    tft_fillCircle(x, y, 20, ILI9340_BLACK);

    if (isTouched)

    {
        // tft_fillCircle(x, y, 20, ILI9340_BLACK);
        // tft_drawChar(x - 10, y - 10, 'x', ILI9340_BLACK, ILI9340_BLACK, 10);
        tft_fillRect(x - 5, y - 1, 10, 2, ILI9340_BLACK);
        tft_fillRect(x - 1, y - 5, 2, 10, ILI9340_BLACK);

        x = newX;
        y = newY;
    }
    // tft_fillCircle(x, y, 20, ILI9340_WHITE);
    // tft_drawChar(x - 10, y - 10, 'x', ILI9340_WHITE, ILI9340_WHITE, 10);
    tft_fillRect(x - 5, y - 1, 10, 2, ILI9340_WHITE);
    tft_fillRect(x - 1, y - 5, 2, 10, ILI9340_WHITE);
    tft_setCursor(20, 100);
    tft_setTextColor(ILI9340_WHITE);
    sprintf(buffer, "x: %d, y: %d", x, y);
    tft_writeString(buffer);
}
