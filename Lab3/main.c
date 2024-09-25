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

uint32_t current_time, debounce_time, game_time;

void main()
{
    uint8_t ledout = 0b00000000;
    sw_in_init();
    debounce_sw1_init();
    debounce_sw2_init();
    game_init();
    led_out_init();

    debounce_time = game_time = timer_read();

    led_out_write(led_state());

    // S0 wait for start button
    // S1 Move the led from left to right
    // S2 Move the led from right to left
    // S3 LED is all the way right, if we dont have a button press the game is over -> S5
    // S4 LED is all the way left, if we dont have a button press the game is over -> S6
    // S5 Game over, right side lost -> S0
    // S6 Game over, left side lost -> S0

    enum State
    {
        GAMESTART,
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT,
        RIGHT_SIDE,
        LEFT_SIDE,
        LOST_RIGHT,
        LOST_LEFT
    };

    enum State state = GAMESTART;
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
