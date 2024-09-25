#include "pico/stdlib.h"
#include "led_out.h"
#include "stdint.h"
#include "sw_in.h"
#include "stdbool.h"
#include "stdlib.h"
#include "debounce_sw1.h"
#include "debounce_sw2.h"
#include "timer.h"
#include "game.h"
#include "hardware/uart.h"

uint32_t current_time, debounce_time, game_time;

void main()
{
    uint8_t ledout = 0b00000000;
    sw_in_init();
    debounce_sw1_init();
    debounce_sw2_init();
    game_init();
    led_out_init();

    uart_init(uart0, 115200);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);

    debounce_time = game_time = timer_read();

    led_out_write(led_state());

    while (1)
    {
        current_time = timer_read();
        if (timer_elapsed_ms(debounce_time, current_time) >= DEBOUNCE_PD_MS)
        {
            debounce_sw1_tick();
            debounce_sw2_tick();
            // game_tick(debounce_sw1_pressed(), debounce_sw2_pressed());

            debounce_time = current_time;
        }

        if (timer_elapsed_ms(game_time, current_time) >= game_speed())
        {
            game_tick(debounce_sw1_pressed(), debounce_sw2_pressed());
            game_time = current_time;
        }

        led_out_write(led_state());
    }
}
