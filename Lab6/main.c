#include "pico/stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "ts_lcd.h"
#include "limits.h"
#include "timer.h"
#include "pwm_pin.h"
#include "stdio.h"
#include "ic.h"

uint32_t current_time, last_update_time, last_rpm_time, display_timer, pid_timer;

static inline int32_t max(uint32_t a, uint32_t b)
{
    if (a > b)
        return a;
    else
        return b;
}

static inline int32_t min(uint32_t a, uint32_t b)
{
    if (a < b)
        return a;
    else
        return b;
}

void main()
{
    stdio_init_all(); // stdo all ports

    uint16_t pwm_level = 0x8000; // start with a 50% duty cycle
    pwm_pin_init(pwm_level);     // init PWM
    printf("Setting OC limit to %x\n", pwm_level);

    ic_init(); // init the input capture to get the rpm speed

    ts_lcd_init(); // init the lcd

    current_time = last_update_time = last_rpm_time = timer_read(); // init timer values

    int pwmHistogram[32] = {0};
    int rpmHistogram[32] = {0};

    uint16_t desiredRPM = 1000;
    uint16_t currentRPM = 0;
    uint16_t previousRPM = 0;

    while (1)
    {
        current_time = timer_read();

        // ts_test();

        int updateTime = 3000 / 32;
        if (timer_elapsed_ms(last_update_time, current_time) >= updateTime)
        {

            for (int i = 31; i > 0; i--)
            {
                pwmHistogram[i] = pwmHistogram[i - 1];
            }
            pwmHistogram[0] = pwm_level;

            last_update_time = current_time;
        }

        if (timer_elapsed_ms(last_rpm_time, current_time) >= 1000)
        {
            uint32_t rpmVal = ic_getrpm();
            if (rpmVal > 10000)
            {
                rpmVal = 0;
            }
            printf("rpm=%u\n", rpmVal);

            for (int i = 31; i > 0; i--)
            {
                rpmHistogram[i] = rpmHistogram[i - 1];
            }
            rpmHistogram[0] = rpmVal;

            last_rpm_time = current_time;
        }

        if (timer_elapsed_ms(display_timer, current_time) >= 0)
        {
            clearDisplay();
            displayHistogram(pwmHistogram, 0);
            displayHistogram(rpmHistogram, 1);

            display_timer = current_time;
        }

        if (timer_elapsed_ms(display_timer, pid_timer) >= 50)
        {
            uint16_t currentRPM = ic_getrpm();
            uint16_t error = currentRPM - desiredRPM;
            uint16_t deriv = previousRPM - currentRPM;

            uint16_t Kp = 500;
            uint16_t Kd = 1000;

            pwm_level = Kp * error - Kd * deriv;

            previousRPM = currentRPM;
            display_timer = current_time;
        }

        if (uart_is_readable(uart0))
        {
            char c = getchar();
            putchar(c);
            if (c == '+')
            {
                pwm_level = min(pwm_level + 0x1000, 0xffff);
            }
            else if (c == '-')
            {
                pwm_level = max(pwm_level - 0x1000, 0);
            }
            else if (c == '0')
            {
                pwm_level = 0;
            }
            else if (c == 'f')
            {
                pwm_level = 0xffff;
            }
            else
                printf("?\n");
            printf("\nSetting PWM level to 0x%x\n", pwm_level);
            pwm_pin_set_level(pwm_level);
        }
    }
}
