#include "../defs.h"
#include "../gpio_config/gpio_config_io.c"

// --------------------------------------------------------
// Firmware routines
// --------------------------------------------------------

void set_registers();
void printstr(char *str);
void printhex(uint32_t x, bool newline);
void printchr(char x);


uint32_t get_prog(uint32_t pc);

void setup_test() {
    reg_gpio_mode1 = 1;
    reg_gpio_mode0 = 0;
    reg_gpio_ien = 1;
    reg_gpio_oe = 1;

    //reg_uart_clkdiv = 10417;
    reg_uart_enable = 1;

    reg_mprj_xfer = 1;

    set_registers();
    reg_mprj_datah = 0;
    reg_mprj_datal = 0;
    gpio_config_io();

    reg_gpio_out = 1; // OFF

    reg_wb_enable = 1;
}

void main()
{
    setup_test();
    printstr("hello\n");

   	// Configure LA[63:0] as core -> caravel
	// Configure LA[127:64] as caravel -> core
	reg_la0_oenb = reg_la0_iena = 0x00000000;    // [31:0]
	reg_la1_oenb = reg_la1_iena = 0x00000000;    // [63:32]
	reg_la2_oenb = reg_la2_iena = 0xffffffff;    // [95:64]
	reg_la3_oenb = reg_la3_iena = 0xffffffff;    // [127:96] 

    // First test - small data
    printstr("gmorn\n");

	// SHA256 ident
	for (int i = 0; i < 64; i++) {
		reg_uart_data=(*(char*)(0x30010050+i));
		while(reg_uart_txfull==1);
		delay(50000);
	}

	// AES ident
	for (int i = 0; i < 64; i++) {
		reg_uart_data=(*(char*)(0x30000050+i));
		while(reg_uart_txfull==1);
		delay(50000);
	}
}

void printstr(char *str) {
    while (*str) printchr(*str++);
}

void printhex(uint32_t x, bool newline) {
    char *s = "0123456789abcdef";

    for (int i = 0; i < 8; i++) {
        printchr(s[(x & 0xF0000000) >> 28]);
        x = x << 4;
    }
    if(newline) printchr('\n');
}

void printchr(char x) {
    while(reg_uart_txfull==1);
    reg_uart_data=x;
    while(reg_uart_txfull==1);
}

void set_registers() {

    reg_mprj_io_0 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_1 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_2 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_3 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_4 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_5 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_6 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_7 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_8 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_9 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_10 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_11 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_12 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_13 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_14 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_15 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_16 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_17 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_18 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;

    reg_mprj_io_19 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_20 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_21 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_22 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_23 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_24 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_25 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_26 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_27 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_28 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_29 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_30 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_31 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_32 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_33 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_34 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_35 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_36 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_37 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;

}


