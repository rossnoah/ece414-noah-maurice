
#include "pico/stdlib.h"
#include "led_out.h"
#include "stdint.h"
#include "sw_in.h"
#include "stdbool.h"
#include "stdlib.h"

bool inLeft, inRight;
uint16_t out;
bool turn = 1;
uint8_t led_pos = 0;

// void assignRandomTurn()

int main()
{
    turn = rand() % 2;
    led_pos = turn == 0 ? 0b10000000 : 0b00000001;

    led_out_init();
    sw_in_init();
    // S0 wait for start button
    // S1 Move the led from left to right
    // S2 Move the led from right to left
    // S3 LED is all the way right, if we dont have a button press the game is over -> S5
    // S4 LED is all the way left, if we dont have a button press the game is over -> S6
    // S5 Game over, right side lost -> S0
    // S6 Game over, left side lost -> S0

    enum State
    {
        GAMESTART,
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT,
        RIGHT_SIDE,
        LEFT_SIDE,
        LOST_RIGHT,
        LOST_LEFT
    };

    enum State state = GAMESTART;

    while (1)
    {
        led_out_write(led_pos);

        inLeft = sw_in_read1();
        inRight = sw_in_read2();

        switch (state)
        {
        case GAMESTART:
            /* code */
            if (turn == 0)
            {
                if (inLeft)
                {
                    state = LEFT_TO_RIGHT;
                }
            }
            else
            {
                if (inRight)
                {
                    state = RIGHT_TO_LEFT;
                }
            }

            led_pos = turn == 0 ? 0b10000000 : 0b00000001;

            break;
        case LEFT_TO_RIGHT:
            if (led_pos == 0b00000001)
            {
                state = RIGHT_SIDE;
                break;
            }
            led_pos = led_pos >> 1;
            sleep_ms(800);
            break;
        case RIGHT_TO_LEFT:
            if (led_pos == 0b10000000)
            {
                state = LEFT_SIDE;
                break;
            }
            led_pos = led_pos << 1;
            sleep_ms(800);
            break;
        case RIGHT_SIDE:
            sleep_ms(500);
            if (inRight)
            {
                state = RIGHT_TO_LEFT;
            }
            else
            {
                state = LOST_RIGHT;
            }
            break;
        case LEFT_SIDE:
            sleep_ms(500);

            if (inLeft)
            {
                state = LEFT_TO_RIGHT;
            }
            else
            {
                state = LOST_LEFT;
            }
            break;
        case LOST_RIGHT:
            led_out_write(0b00000111);
            sleep_ms(1000);
            state = GAMESTART;
            break;
        case LOST_LEFT:
            led_out_write(0b11100000);
            sleep_ms(1000);
            state = GAMESTART;
            break;

        default:
            break;
        }
        led_out_write(led_pos);
    }
    return 0;
}

void assignRandomTurn()
{
    turn = rand() % 2;
}