#include "pico/stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "ts_lcd.h"

void main()
{

    ts_lcd_init();

    while (1)
    {

        ts_test();
    }
}
