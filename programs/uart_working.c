#include "../defs.h"
#include "../gpio_config/gpio_config_io.c"

#define UART_EV_TX	0x1
#define UART_EV_RX	0x2

#define UART_RXTX    (*(volatile uint32_t*) 0xf0005800)
#define UART_TXFULL  (*(volatile uint32_t*) 0xf0005804)
#define UART_RXEMPTY (*(volatile uint32_t*) 0xf0005808)
#define UART_EV_PEND (*(volatile uint32_t*) 0xf0005810)

void putchar(char x) {
    // Wait until ready
    while(UART_TXFULL);
    
    // Write character
    UART_RXTX = x;

    // Wait until send completed
    while(UART_TXFULL);
}

bool haschar() {
    return !UART_RXEMPTY;
}

char getchar() {
    while (!haschar());

    // Read the data
    uint8_t c = UART_RXTX;
    
    // Clear the data out of the FIFO
    UART_EV_PEND = UART_EV_RX;

    return c;
}

void main() {
    // Enable the GPIO LED
    reg_gpio_mode1 = 1;
    reg_gpio_mode0 = 0;
    reg_gpio_ien = 1;
    reg_gpio_oe = 1;
    reg_gpio_out = 1;

    // Enable UART
    reg_uart_enable = 1;

    // Setup the I/O
    reg_mprj_xfer = 1;
    reg_mprj_io_5 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_6 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_datah = 0;
    reg_mprj_datal = 0;
    gpio_config_io();

    while (true) {
        char c = getchar();
        
        // Toggle LED every time character recieved
        reg_gpio_out = !reg_gpio_out;

        // Write the recieved character + 1
        putchar(c+1);
    }
}
