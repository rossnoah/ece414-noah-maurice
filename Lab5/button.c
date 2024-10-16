// #include "button.h"
// #include "stdint.h"
// #include "stdbool.h"

// void init_button(Button *button)
// {
//     button->pressed = false;
//     button->cooldown = 0;
// }

// bool is_pressed(Button *button)
// {
//     if (button->pressed)
//     {
//         button->cooldown = BUTTON_PRESSED_MS;
//         return true;
//     }
//     return false;
// }

// void tick_button(Button *button)
// {
//     if (button->cooldown > 0)
//     {
//         button->cooldown--;
//     }
// }

// bool is_in_area(Button *button, int16_t x, int16_t y)
// {
// }
