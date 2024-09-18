#include "pico/stdlib.h"
#include "led_out.h"
#include "stdint.h"
#include "stdbool.h"
#include "sw_in.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define LED_PIN 8
int main()
{

    bool in1, in2;
    uint16_t out;
    led_out_init();
    sw_in_init();

    while (1)
    {
        // gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
        // gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
        uart_puts(UART_ID, "\n\nHello! is this thing on?\n");
        printf("printf demands to be seen and heard!\n\n");
        while (true)
        {
            if (uart_is_readable(UART_ID))
            {
                char c = uart_getc(UART_ID);
                uart_putc(UART_ID, c + 1);
            }
            gpio_put(LED_PIN, 1);
            sleep_ms(500);
            gpio_put(LED_PIN, 0);
            sleep_ms(500);
            uart_putc(uart0, '#');
        }
        return 0;
    }
}