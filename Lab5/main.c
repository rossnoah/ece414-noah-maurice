#include "pico/stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "ts_lcd.h"
#include "limits.h"
#include "button.h"
enum OPPERATOR
{
    ADD,
    SUB,
    MUL,
    DIV,
    CLEAR,
    EQUAL,
};

struct calculator
{
    int32_t opperand1;
    int32_t opperand2;
    enum OPPERATOR operator;
    char display[30];
    bool hasError;
};

void addToOpperand(int32_t *opperand, int value)
{

    if (*opperand * 10 + value > INT32_MAX)
    {
        return; // dont do anything because we are at the max number so just ignore the input
    }
    *opperand = *opperand * 10 + value; // set the value
}

// bool pressed;
// int32_t cooldown;
// int32_t x, y, w, h;
// unsigned short unpressed_color, pressed_color;
// void (*on_press)();

int btnCnt = 0;

struct Button buttons[16];

struct Button b1 = {false, 0, 0, 90, 67, 100, '7'};
struct Button b2 = {false, 90, 0, 90, 67, 100, '8'};
struct Button b3 = {false, 180, 0, 90, 67, 100, '9'};

void initButton(struct Button b)
{
    buttons[btnCnt] = b;
    btnCnt++;
}
void main()
{

    initButton(b1);
    initButton(b2);
    initButton(b3);

    ts_lcd_init();

    while (1)
    {

        ts_test();

        for (int i = 0; i < btnCnt; i++)
        {
            render_button(buttons[i]);
        }
        sleep_ms(100);
    }
}
