#include "sw_in.h"
#include "stdbool.h"

#define INPUT_PIN_2 22
#define INPUT_PIN_1 21

void sw_in_init()
{
    // do thing
    gpio_init(INPUT_PIN_1);
    gpio_init(INPUT_PIN_2);

    gpio_set_dir(INPUT_PIN_1, GPIO_IN);
    gpio_set_dir(INPUT_PIN_2, GPIO_IN);

    gpio_pull_up(INPUT_PIN_1);
    gpio_pull_up(INPUT_PIN_2);
}
bool sw_in_read1()
{
    return !gpio_get(INPUT_PIN_1);
}

bool sw_in_read2()
{
    return !gpio_get(INPUT_PIN_2);
}
