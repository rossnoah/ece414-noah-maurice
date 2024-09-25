/*
 * File:   debounce_sw2.c
 * Author: nestorj
 *
 * Debounce a switch read by the sw_in module
 */

#include "stdio.h"
#include "pico/stdlib.h"
#include "debounce_sw2.h"
#include "sw_in.h"

// flag indicating button pressed and debounced
// cleared when read by debounce1_pressed()
static bool btn_pressed;

// state variable

static enum Game_States { NOPUSH,
                          MAYBEPUSH,
                          PUSHED,
                          MAYBENOPUSH } Game_State;

void debounce_sw2_init()
{
    Game_State = NOPUSH;
    btn_pressed = false;
}

void debounce_sw2_tick()
{
    bool btn = sw_in_read2();
    switch (Game_State)
    {
    case NOPUSH:
        if (btn)
            Game_State = MAYBEPUSH;
        else
            Game_State = NOPUSH;
        break;
    case MAYBEPUSH:
        if (btn)
        {
            btn_pressed = true;
            Game_State = PUSHED;
        }
        else
            Game_State = NOPUSH;
        break;
    case PUSHED:
        if (btn)
            Game_State = PUSHED;
        else
            Game_State = MAYBENOPUSH;
        break;
    case MAYBENOPUSH:
        if (btn)
            Game_State = PUSHED;
        else
            Game_State = NOPUSH;
        break;
    default:
        Game_State = NOPUSH;
        break;
    }

    // note: no other state actions required,
    // so we don't need a 2nd switch statement
}

// return TRUE the first time the function is called after the button has
// been pressed.  Return FALSE until the button is released and pressed again
bool debounce_sw2_pressed()
{
    if (btn_pressed)
    {
        btn_pressed = false;
        return true;
    }
    else
        return false;
}
