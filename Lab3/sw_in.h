#ifndef SW_IN_H
#define SW_IN_H

#include "pico/stdlib.h"
#include "stdbool.h"

#define INPUT_PIN_2 22
#define INPUT_PIN_1 21

void sw_in_init();
bool sw_in_read1();
bool sw_in_read2();

#endif