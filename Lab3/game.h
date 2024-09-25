
#ifndef GAME_H
#define GAME_H
#include "stdbool.h"
#include "pico/stdlib.h"

#define GAME_MS 300

void game_init();

void game_tick();

int led_state();

int game_speed();

#endif
