#include "pico/stdlib.h"
#include "led_out.h"
#include "stdint.h"
#include "sw_in.h"
#include "stdbool.h"
#include "stdlib.h"
#include "debounce_sw1.h"
#include "debounce_sw2.h"

uint16_t out;
bool turn = 1;
uint8_t led_pos = 0;

uint32_t t1, t2;

void main()
{
    uint32_t t1, t2;
    uint8_t ledout = 0b00000000;
    sw_in_init();
    debounce_sw1_init();
    debounce_sw2_init();
    led_out_init();

    t1 = timer_read();
    printf("getting started\n");
    while (1)
    {
        t2 = timer_read();
        if (timer_elapsed_ms(t1, t2) >= DEBOUNCE_PD_MS)
        {
            debounce_sw1_tick();
            debounce_sw2_tick();
            t1 = t2;
        }

        if (debounce_sw1_pressed() && debounce_sw2_pressed())

        {
            led_out_write(0xff);
        }
        else if (debounce_sw1_pressed())
        {
            led_out_write(0b10000000);
        }
        else if (debounce_sw2_pressed())
        {
            led_out_write(0b00000001);
        }
        else
        {
            led_out_write(0b00000000);
        }
    }
}

// void tick();
// int main()
// {
//     turn = rand() % 2;
//     led_pos = turn ? 0b00000001 : 0b10000000;

//     led_out_init();
//     sw_in_init();

//     led_out_write(led_pos);
//     // S0 wait for start button
//     // S1 Move the led from left to right
//     // S2 Move the led from right to left
//     // S3 LED is all the way right, if we dont have a button press the game is over -> S5
//     // S4 LED is all the way left, if we dont have a button press the game is over -> S6
//     // S5 Game over, right side lost -> S0
//     // S6 Game over, left side lost -> S0

//     enum State
//     {
//         GAMESTART,
//         LEFT_TO_RIGHT,
//         RIGHT_TO_LEFT,
//         RIGHT_SIDE,
//         LEFT_SIDE,
//         LOST_RIGHT,
//         LOST_LEFT
//     };

//     enum State state = GAMESTART;

//     while (1)
//     {
//         tick();
//         // if (inLeft || inRight)
//         // {
//         //     led_out_write(0b00001111);
//         // }
//         // else
//         // {
//         //     led_out_write(0b11110000);
//         // }

//         switch (state)
//         {
//         case GAMESTART:
//             /* code */
//             if (turn == 0)
//             {
//                 if (inLeft)
//                 {
//                     state = LEFT_TO_RIGHT;
//                 }
//             }
//             else
//             {
//                 if (inRight)
//                 {
//                     state = RIGHT_TO_LEFT;
//                 }
//             }

//             led_pos = turn ? 0b00000001 : 0b10000000;
//             led_out_write(led_pos);

//             break;
//         case LEFT_TO_RIGHT:
//             if (led_pos == 0b00000001)
//             {
//                 state = RIGHT_SIDE;
//                 break;
//             }
//             led_pos = led_pos >> 1;
//             led_out_write(led_pos);
//             sleep_ms(800);
//             break;
//         case RIGHT_TO_LEFT:
//             if (led_pos == 0b10000000)
//             {
//                 state = LEFT_SIDE;
//                 break;
//             }
//             led_pos = led_pos << 1;
//             led_out_write(led_pos);
//             sleep_ms(800);
//             break;
//         case RIGHT_SIDE:
//             if (inRight)
//             {
//                 state = RIGHT_TO_LEFT;
//             }
//             else
//             {
//                 state = LOST_RIGHT;
//                 turn = rand() % 2;
//             }
//             break;
//         case LEFT_SIDE:
//             if (inLeft)
//             {
//                 state = LEFT_TO_RIGHT;
//             }
//             else
//             {
//                 state = LOST_LEFT;
//                 turn = rand() % 2;
//             }
//             break;
//         case LOST_RIGHT:
//             led_out_write(0b00000111);
//             sleep_ms(3000);
//             state = GAMESTART;
//             break;
//         case LOST_LEFT:
//             led_out_write(0b11100000);
//             sleep_ms(3000);
//             state = GAMESTART;
//             break;

//         default:
//             break;
//         }
//     }
//     return 0;
// }

// void assignRandomTurn()
// {
//     turn = rand() % 2;
// }

// void tick()
// {
//     sleep_ms(1);
//     inLeft = sw_in_read1();
//     inRight = sw_in_read2();

//     if (inLeft && leftCtn == 0)
//     {
//         leftBtn = true;
//         leftCtn = 200;
//     }

//     if (leftCtn > 0)
//     {
//         leftCtn--;
//     }

//     if (leftCtn == 0)
//     {
//         leftBtn = false;
//     }

//     if (inRight && rightCtn == 0)
//     {
//         rightBtn = true;
//         rightCtn = 200;
//     }

//     if (rightCtn > 0)
//     {
//         rightCtn--;
//     }

//     if (rightCtn == 0)
//     {
//         rightBtn = false;
//     }
// }