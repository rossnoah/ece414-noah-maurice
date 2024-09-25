/*
 * File:   debounce_sw1.h
 * Author: nestorj
 *
 * Debounce a switch read by the sw_in module
 */

#ifndef GAME_H
#define GAME_H
#include "stdbool.h"
#include "pico/stdlib.h"

// use to schedule in main function:
#define GAME_MS 400

void game_init();

void game_tick();

// return TRUE the first time the function is called after the button has
// been pressed.  Return FALSE until the button is released and pressed again
int led_state();

#endif /* DEBOUNCER_H */
