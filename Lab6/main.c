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

int current_time, last_update_time, last_rpm_time, display_timer, pid_timer;

static inline int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

static inline int min(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}

char input_state;

void main()
{
    input_state = 'x';
    stdio_init_all(); // stdo all ports

    int pwm_level = 0x8000;  // start with a 50% duty cycle
    pwm_pin_init(pwm_level); // init PWM
    // printf("Setting OC limit to %x\n", pwm_level);
    printf("Press S, P, I, D to adjust speed, p, i, d values.");

    ic_init(); // init the input capture to get the rpm speed

    ts_lcd_init(); // init the lcd

    current_time = last_update_time = last_rpm_time = timer_read(); // init timer values

    int pwmHistogram[32] = {0};
    int rpmHistogram[32] = {0};

    double desiredRPM = 1000;
    int currentRPM = 0;
    int previousRPM = 0;
    int integral = 0;

    double Kp = 20;
    double Kd = 30;
    double Ki = 1.5;

    int integral_limit = 10000;

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
            int rpmVal = currentRPM;
            if (rpmVal > 10000)
            {
                rpmVal = 0;
            }
            // printf("rpm=%u\n", rpmVal);

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

        if (timer_elapsed_ms(pid_timer, current_time) >= 0)
        {
            currentRPM = ic_getrpm();
            if ((previousRPM != 0 && currentRPM == 0) || currentRPM > 4000)
            {
                currentRPM = previousRPM;
            }
            int error = desiredRPM - currentRPM;
            int deriv = currentRPM - previousRPM;
            integral += error;
            integral = min(max(integral, -integral_limit), integral_limit);

            int pid_output = Kp * error + Ki * integral - Kd * deriv;
            pwm_level += pid_output;

            if (pwm_level < 0)
                pwm_level = 0;
            if (pwm_level > 0xffff)
                pwm_level = 0xffff;

            pwm_pin_set_level(pwm_level);

            // printf("desiredrpm: %f\ncurrent rpm %i\nerror: %i\nintegral: %i\npwm_out: %i\nnew pwm:%i\n\n\n", desiredRPM, currentRPM, error, integral, pid_output, pwm_level);
            // printf("p: %f, i: %f, d: %f\n", Kp, Ki, Kd);
            previousRPM = currentRPM;
            pid_timer = current_time;
        }

        if (uart_is_readable(uart0))
        {
            char c = getchar();
            putchar(c);

            if (input_state == 'x')
            {
                printf("\n");
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
                static char input_buffer[32];
                static int input_index = 0;

                if (c >= '0' && c <= '9' || c == '.' || c == '-')
                {
                    if (input_index < sizeof(input_buffer) - 1)
                    {
                        input_buffer[input_index++] = c;
                    }
                }
                else if (c == '\n' || c == ' ' || c == '\r')
                {
                    input_buffer[input_index] = '\0';

                    double value = 0;
                    if (sscanf(input_buffer, "%lf", &value) == 1)
                    {
                        if (input_state == 's')
                        {
                            desiredRPM = value;
                            integral = 0;
                        }
                        else if (input_state == 'p')
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

                    input_index = 0;
                }
            }
        }
    }
}
