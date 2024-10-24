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
    int result = *opperand * 10 + value;
    if (((*opperand > 0) && (result < *opperand)) || result > INT32_MAX)
    {
        return; // dont do anything because we are at the max number so just ignore the input
    }
    *opperand = result; // set the value
}

int btnCnt = 0;

uint32_t current_time, button_debounce_time;

struct Button buttons[16];

void initButton(struct Button b)
{
    buttons[btnCnt] = b;
    btnCnt++;
}
void main()
{

    int32_t opperand1 = 0;

    initButton(createButton(0, 40, 83, 50, '7'));
    initButton(createButton(83, 40, 83, 50, '8'));
    initButton(createButton(83 * 2, 40, 83, 50, '9'));
    initButton(createButton(0, 50 * 1 + 40, 83, 50, '4'));
    initButton(createButton(83, 50 * 1 + 40, 83, 50, '5'));
    initButton(createButton(83 * 2, 50 * 1 + 40, 83, 50, '6'));

    initButton(createButton(0, 50 * 2 + 40, 83, 50, '1'));
    initButton(createButton(83, 50 * 2 + 40, 83, 50, '2'));
    initButton(createButton(83 * 2, 50 * 2 + 40, 83, 50, '3'));
    initButton(createButton(0, 50 * 3 + 40, 83, 50, '0'));
    // the next set of buttons
    initButton(createButton(83 * 3, 50 * 0 + 40, 83, 50, '+'));
    initButton(createButton(83 * 3, 50 * 1 + 40, 83, 50, '-'));
    initButton(createButton(83 * 3, 50 * 2 + 40, 83, 50, '/'));
    initButton(createButton(83 * 3, 50 * 3 + 40, 83, 50, 'x'));
    initButton(createButton(83 * 2, 50 * 3 + 40, 83, 50, 'C'));
    initButton(createButton(83 * 1, 50 * 3 + 40, 83, 50, '='));
    ts_lcd_init();

    current_time = timer_read();
    for (int i = 0; i < btnCnt; i++)
    {
        buttons[i].time = current_time;
    }

    while (1)
    {

        current_time = timer_read();
        uint16_t x, y;
        bool isTouched = get_ts_lcd(&x, &y);
        for (int i = 0; i < btnCnt; i++)
        {
            if (timer_elapsed_ms(buttons[i].time, current_time) >= BUTTON_PRESSED_MS)
            {
                tick_button(&buttons[i], x, y, isTouched);
                button_debounce_time = current_time;
            }
        }

        for (int i = 0; i < btnCnt; i++)
        {
            if (buttons[i].pressed)
            {

                int value = buttons[i].c & 0b00001111;
                addToOpperand(&opperand1, value);
            }
        }

        for (int i = 0; i < btnCnt; i++)
        {
            render_button(&buttons[i]);
        }

        render_text(&opperand1);

        // ts_test();
    }
}
