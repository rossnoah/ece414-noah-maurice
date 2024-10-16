

#include "stdint.h"
#include "stdbool.h"
#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_PRESSED_MS 100

struct Button
{
    bool pressed;
    int32_t x, y, w, h;
    int32_t cooldown;
    char c;
    // unsigned short unpressed_color, pressed_color;
    // void (*on_press)();
};

void init_button(struct Button *button);

bool is_pressed(struct Button *button);

bool is_in_area(struct Button *button, int16_t x, int16_t y);

void tick_button(struct Button *button);

#endif
