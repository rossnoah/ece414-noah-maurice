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

// flag indicating button pressed and debounced
// cleared when read by debounce1_pressed()
static int turn;
static int led_pos;
static int game_speed_count;

// Game_State variable

static enum Game_States { GAMESTART,
                          LEFT_TO_RIGHT,
                          RIGHT_TO_LEFT,
                          RIGHT_SIDE,
                          LEFT_SIDE,
                          LOST_RIGHT,
                          LOST_LEFT } Game_State;

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
                Game_State = LEFT_TO_RIGHT;
            }
        }
        else
        {
            if (btn2)
            {
                Game_State = RIGHT_TO_LEFT;
            }
        }

        led_pos = turn ? 0b00000001 : 0b10000000;

        break;
    case LEFT_TO_RIGHT:
        led_pos = led_pos >> 1;
        if (led_pos == 0b00000001)
        {
            Game_State = RIGHT_SIDE;
        }
        break;

    case RIGHT_TO_LEFT:
        led_pos = led_pos << 1;
        if (led_pos == 0b10000000)
        {
            Game_State = LEFT_SIDE;
        }
        break;

    case RIGHT_SIDE:
        if (btn2)
        {
            Game_State = RIGHT_TO_LEFT;
            led_pos = led_pos << 1;
            game_speed_count++;
        }
        else
        {
            Game_State = LOST_RIGHT;
            turn = rand() % 2;
        }
        break;

    case LEFT_SIDE:
        if (btn1)
        {
            Game_State = LEFT_TO_RIGHT;
            led_pos = led_pos >> 1;
            game_speed_count++;
        }
        else
        {
            Game_State = LOST_LEFT;
            turn = rand() % 2;
        }
        break;

    case LOST_RIGHT:
        led_out_write(0x0f);
        sleep_ms(300);
        led_out_write(0x00);
        sleep_ms(300);
        led_out_write(0x0f);
        sleep_ms(300);
        Game_State = GAMESTART;
        game_speed_count = 0;
        break;

    case LOST_LEFT:
        led_out_write(0xf0);
        sleep_ms(300);
        led_out_write(0x00);
        sleep_ms(300);
        led_out_write(0xf0);
        sleep_ms(300);
        Game_State = GAMESTART;
        game_speed_count = 0;

        break;

    default:
        break;
    }

    // note: no other Game_State actions required,
    // so we don't need a 2nd switch Game_Statement
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