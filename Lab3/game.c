/*
 * File:   debounce_sw1.c
 * Author: nestorj
 *
 * Debounce a switch read by the sw_in module
 */

#include "stdio.h"
#include "pico/stdlib.h"
#include "debounce_sw1.h"
#include "sw_in.h"

// flag indicating button pressed and debounced
// cleared when read by debounce1_pressed()
static int turn;
static int led_pos;

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
    led_pos = turn ? 0b00000001 : 0b10000000;
}

void game_tick(bool btn1, bool btn2)
{
    bool btn = sw_in_read1();
    switch (Game_State)
    {
    case GAMESTART:
        /* code */
        if (turn == 0)
        {
            if (debounce_sw1_pressed())
            {
                Game_State = LEFT_TO_RIGHT;
            }
        }
        else
        {
            if (debounce_sw2_pressed())
            {
                Game_State = RIGHT_TO_LEFT;
            }
        }

        led_pos = turn ? 0b00000001 : 0b10000000;
        led_out_write(led_pos);

        break;
    case LEFT_TO_RIGHT:
        if (led_pos == 0b00000001)
        {
            Game_State = RIGHT_SIDE;
            break;
        }
        led_pos = led_pos >> 1;
        led_out_write(led_pos);
        break;
    case RIGHT_TO_LEFT:
        if (led_pos == 0b10000000)
        {
            Game_State = LEFT_SIDE;
            break;
        }
        led_pos = led_pos << 1;
        led_out_write(led_pos);
        break;
    case RIGHT_SIDE:
        if (debounce_sw2_pressed())
        {
            Game_State = RIGHT_TO_LEFT;
        }
        else
        {
            Game_State = LOST_RIGHT;
            turn = rand() % 2;
        }
        break;
    case LEFT_SIDE:
        if (debounce_sw1_pressed())
        {
            Game_State = LEFT_TO_RIGHT;
        }
        else
        {
            Game_State = LOST_LEFT;
            turn = rand() % 2;
        }
        break;
    case LOST_RIGHT:
        led_pos = 0b00000111;
        Game_State = GAMESTART;
        break;
    case LOST_LEFT:
        led_pos = 0b11100000;
        Game_State = GAMESTART;
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