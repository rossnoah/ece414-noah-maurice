#include "pico/stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "ts_lcd.h"
#include "limits.h"
#include "button.h"
#include "timer.h"

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

uint32_t current_time, button_debounce_time;

struct Button buttons[16];

// struct Button b1 = {false, 0, 0, 90, 67, 100, '7'};
// struct Button b2 = {false, 90, 0, 90, 67, 100, '8'};
// struct Button b3 = {false, 180, 0, 90, 67, 100, '9'};

int x, y;
bool isTouched;

void initButton(struct Button b)
{
    buttons[btnCnt] = b;
    btnCnt++;
}
void main()
{
    x = y = 0;
    isTouched = false;

    // initButton(b1);
    // initButton(b2);
    // initButton(b3);
    // struct Button b1 = {false, 0, 0, 90, 67, 100, '7'};
    // struct Button b2 = {false, 90, 0, 90, 67, 100, '8'};
    // struct Button b3 = {false, 180, 0, 90, 67, 100, '9'};
    initButton(createButton(0, 0, 90, 67, '7'));
    initButton(createButton(90, 0, 90, 67, '8'));
    initButton(createButton(180, 0, 90, 67, '9'));
    ts_lcd_init();

    current_time = timer_read();
    for (int i = 0; i < btnCnt; i++)
    {
        buttons[i].time = current_time;
    }

    while (1)
    {

        current_time = timer_read();
        for (int i = 0; i < btnCnt; i++)
        {
            if (timer_elapsed_ms(buttons[i].time, current_time) >= BUTTON_PRESSED_MS)
            {
                tick_button(buttons[i], x, y, isTouched);
                button_debounce_time = current_time;
            }
        }

        for (int i = 0; i < btnCnt; i++)
        {
            render_button(buttons[i]);
        }

        ts_test();
    }
}
