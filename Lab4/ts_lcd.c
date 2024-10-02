#include "stdint.h"
#include "TFTMaster.h"
#include "TouchScreen.h"

struct TSPoint p;
char buffer[30];
uint16_t x, y;

bool get_ts_lcd(uint16_t *px, uint16_t *py)
{

    getPoint(&p);

    &px = (px * 240) / 4096;
    &py = (y * 320) / 4096;

    if (p.z)
    {
        return true;
    }
    return false;
}

void ts_lcd_init()
{
    tft_init_hw();
    tft_begin();

    p.x = 0;
    p.y = 0;
    p.z = 0;

    getPoint(&p);
}

void ts_test()
{
    uint16_t newX, newY;
    bool isTouched = get_ts_lcd(&newX, &newY);

    if (isTouched)
    {
        x = newX;
        y = newY;
    }
    else
    {
        return;
    }

    // black screen
    tft_setCursor(20, 100);
    tft_setTextColor(ILI9340_WHITE);
    tft_setTextSize(2);

    // erase old text
    tft_setTextColor(ILI9340_BLACK);
    tft_writeString(buffer);

    tft_setCursor(20, 100);
    tft_setTextColor(ILI9340_WHITE);
    sprintf(buffer, "x: %d, y: %d", x, y);
    tft_writeString(buffer);
}
