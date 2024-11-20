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

char input_state = 'x';

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
    int32_t integral = 0;

    uint16_t Kp = 200;
    uint16_t Kd = 50;
    uint16_t Ki = 10;

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

        if (timer_elapsed_ms(pid_timer, current_time) >= 50)
        {
            currentRPM = ic_getrpm();
            int32_t error = desiredRPM - currentRPM;
            int32_t deriv = currentRPM - previousRPM;
            integral += error;

            int32_t pid_output = Kp * error + Ki * integral - Kd * deriv;
            pwm_level = min(max(pwm_level + pid_output, 0), 0xffff);

            previousRPM = currentRPM;
            pid_timer = current_time;
        }

        if (uart_is_readable(uart0))
        {
            char c = getchar();
            putchar(c);

            if (input_state == 'x')
            {
                if (c == 's')
                {
                    input_state = 's';
                    printf("Enter RPM Value:\n");
                }
                else if (c == 'p')
                {
                    input_state = 'p';
                    printf("Enter Kp Value:\n");
                }
                else if (c == 'i')
                {
                    input_state = 'i';
                    printf("Enter Ki Value:\n");
                }
                else if (c == 'd')
                {
                    input_state = 'd';
                    printf("Enter Kd Value:\n");
                }
            }

            if (input_state != 'x')
            {
                int value = 0;
                if (sscanf(&c, "%d", &value) == 1)
                {

                    if (input_state == 's')
                    {
                        desiredRPM = value;
                    }
                    if (input_state == 'p')
                    {
                        Kp = value;
                    }
                    else if (input_state == 'i')
                    {
                        Ki = value;
                    }
                    else if (input_state == 'd')
                    {
                        Kd = value;
                    }
                    input_state = 'x';
                    printf("\n");
                }
            }
        }
    }
}
