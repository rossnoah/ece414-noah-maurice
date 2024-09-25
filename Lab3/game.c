/*
Game state machine
*/

#include "pico/stdlib.h"
#include "led_out.h"
#include "stdint.h"
#include "sw_in.h"
#include "stdbool.h"
#include "stdlib.h"
#include "debounce_sw1.h"
#include "debounce_sw2.h"
#include "timer.h"
#include "hardware/uart.h"

static int turn = 1;
static int led_pos;
static int game_speed_count;

// Game_State variable

// S0 wait for start button
// S1 Move the led from left to right
// S2 Move the led from right to left
// S3 LED is all the way right, if we dont have a button press the game is over -> S5
// S4 LED is all the way left, if we dont have a button press the game is over -> S6
// S5 Game over, right side lost -> S0
// S6 Game over, left side lost -> S0
static enum Game_States { GAMESTART,
                          R_TO_L,
                          L_TO_R,
                          L_SIDE,
                          R_SIDE,
                          L_LOST,
                          R_LOST } Game_State;

void game_init()
{
    Game_State = GAMESTART;
    turn = rand() % 2;
    turn = rand() % 2;
    led_pos = turn ? 0b00000001 : 0b10000000;
    game_speed_count = 0;
}

void game_tick(bool btn1, bool btn2)
{
    switch (Game_State)
    {
    case GAMESTART:
        /* code */
        if (turn == 0)
        {
            if (btn1)
            {
                Game_State = R_TO_L;
            }
        }
        else
        {
            if (btn2)
            {
                Game_State = L_TO_R;
            }
        }

        led_pos = turn ? 0b00000001 : 0b10000000;

        break;
    case R_TO_L:
        led_pos = led_pos >> 1;
        if (led_pos == 0b00000001)
        {
            Game_State = L_SIDE;
        }
        break;

    case L_TO_R:
        led_pos = led_pos << 1;
        if (led_pos == 0b10000000)
        {
            Game_State = R_SIDE;
        }
        break;

    case L_SIDE:
        if (btn2)
        {
            Game_State = L_TO_R;
            led_pos = led_pos << 1;
            game_speed_count++;
        }
        else
        {
            Game_State = L_LOST;
            turn = rand() % 2;
        }
        break;

    case R_SIDE:
        if (btn1)
        {
            Game_State = R_TO_L;
            led_pos = led_pos >> 1;
            game_speed_count++;
        }
        else
        {
            Game_State = R_LOST;
            turn = rand() % 2;
        }
        break;

    case L_LOST:
        uart_puts(uart0, "Left Lost\n");

        led_out_write(0x80);
        sleep_ms(300);
        led_out_write(0x00);
        sleep_ms(300);
        led_out_write(0x80);
        sleep_ms(300);
        led_out_write(0x00);
        sleep_ms(300);
        led_out_write(0x80);
        sleep_ms(300);
        led_out_write(0x00);
        sleep_ms(300);
        led_out_write(0x80);
        sleep_ms(300);
        Game_State = GAMESTART;
        game_speed_count = 0;
        break;

    case R_LOST:
        uart_puts(uart0, "Right Lost\n");
        led_out_write(0x01);
        sleep_ms(300);
        led_out_write(0x00);
        sleep_ms(300);
        led_out_write(0x01);
        sleep_ms(300);
        led_out_write(0x00);
        sleep_ms(300);
        led_out_write(0x01);
        sleep_ms(300);
        led_out_write(0x00);
        sleep_ms(300);
        led_out_write(0x01);
        sleep_ms(300);

        Game_State = GAMESTART;
        game_speed_count = 0;

        break;

    default:
        break;
    }
}

int led_state()
{
    return led_pos;
}

int game_speed()
{
    switch (game_speed_count)
    {
    case 0:
        return 300;
    case 1:
        return 250;
    case 2:
        return 200;
    case 3:
        return 150;
    default:
        return 100;
    }
}