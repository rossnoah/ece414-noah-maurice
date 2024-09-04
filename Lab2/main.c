#include "pico/stdlib.h"
#include "led_out.h"
#include "stdint.h"
#include "sw_in.h"

int main()
{

    bool in1, in2;
    uint16_t out;
    led_out_init();
    sw_in_init();

    while (1)
    {
        in1 = sw_in_read1();
        in2 = sw_in_read2();

        // do thing

        if (in1 && in2)
        {
            out = 0xff;
        }
        else if (in1 && !in2)
        {
            out = 0x80;
        }
        else if (!in1 && in2)
        {
            out = 0x1;
        }
        else
        {
            out = 0x0;
        }

        led_out_write(out);
    }
    return 0;
}