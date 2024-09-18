#include "led_out.h"
#include "pico/stdlib.h"
#include "stdint.h"
#include "stdbool.h"

void led_out_init()
{

    for (int i = 2; i <= 9; i++) // init on pins 2-9
    {
        gpio_init(i);
        gpio_set_dir(i, GPIO_OUT);
    }
}

void led_out_write(uint8_t val)
{

    // gpio_put(pin, val);

    int count = 0;
    for (int i = 2; i <= 9; i++)
    {
        bool showLed = ((0x1 << count) & val) > 0;

        gpio_put(i, showLed);
        count++;
    }
    // do thing write port
}