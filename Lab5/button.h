

#include "stdint.h"
#include "stdbool.h"
#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_PRESSED_MS 100

typedef struct
{
    bool pressed;
    int32_t cooldown;
    int32_t x, y, w, h;
    unsigned short unpressed_color, pressed_color;
    void (*on_press)();
} Button;

void init_button(Button *button);

bool is_pressed(Button *button);

bool is_in_area(Button *button, int16_t x, int16_t y);

void tick_button(Button *button);

#endif
