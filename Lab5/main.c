#include "pico/stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "ts_lcd.h"

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
    char[30] display;
    bool hasError;
};

void addToOpperand(int32_t *opperand, int value)
{
    int32_t temp = *opperand * 10;
    if (temp < *opperand)
    {
    }
    *opperand = temp + value;
}
*opperand = (*opperand * 10) + value;
}

void main()
{

    ts_lcd_init();

    while (1)
    {

        ts_test();
        sleep_ms(100);
    }
}
