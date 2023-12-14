#include "../defs.h"
#include "../csr.h"
#include "../uart.h"
#include "../gpio_config/gpio_config_io.c"

// --------------------------------------------------------
// Firmware routines
// --------------------------------------------------------

void set_registers();
void printstr(char *str);
void printhex(uint32_t x, bool newline);
void printchr(char x);

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


    // First test - only one data "word"
    printstr("test1\n");
    *((uint32_t*)0x30010000) =  0x1;
    *((uint32_t*)0x30010004) =  0x68656C6C;
    *((uint32_t*)0x30010004) =  0x6F800000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000028;
    while ((*((uint32_t*)0x30010000) & 0x4) != 0x4);

    if (*((uint32_t*)0x30010010) != 0x2CF24DBA) goto fail1;
    if (*((uint32_t*)0x30010014) != 0x5FB0A30E) goto fail1;
    if (*((uint32_t*)0x30010018) != 0x26E83B2A) goto fail1;
    if (*((uint32_t*)0x3001001C) != 0xC5B9E29E) goto fail1;
    if (*((uint32_t*)0x30010020) != 0x1B161E5C) goto fail1;
    if (*((uint32_t*)0x30010024) != 0x1FA7425E) goto fail1;
    if (*((uint32_t*)0x30010028) != 0x73043362) goto fail1;
    if (*((uint32_t*)0x3001002C) != 0x938B9824) goto fail1;
    goto pass1;

    fail1:
    printstr("Res: FAIL\n");
    printhex(*((uint32_t*)0x30010010), true);
    printhex(*((uint32_t*)0x30010014), true);
    printhex(*((uint32_t*)0x30010018), true);
    printhex(*((uint32_t*)0x3001001C), true);
    printhex(*((uint32_t*)0x30010020), true);
    printhex(*((uint32_t*)0x30010024), true);
    printhex(*((uint32_t*)0x30010028), true);
    printhex(*((uint32_t*)0x3001002C), true);
    goto done1;

    pass1:
    printstr("Res: PASS\n");

    done1:

    // Second test - more data
    printstr("test2\n");
    *((uint32_t*)0x30010000) =  0x1;
    *((uint32_t*)0x30010004) =  0x33613537;
    *((uint32_t*)0x30010004) =  0x63643330;
    *((uint32_t*)0x30010004) =  0x35363066;
    *((uint32_t*)0x30010004) =  0x62326637;
    *((uint32_t*)0x30010004) =  0x32383361;
    *((uint32_t*)0x30010004) =  0x37343333;
    *((uint32_t*)0x30010004) =  0x65636539;
    *((uint32_t*)0x30010004) =  0x31393436;
    *((uint32_t*)0x30010004) =  0x61633664;
    *((uint32_t*)0x30010004) =  0x61346234;
    *((uint32_t*)0x30010004) =  0x30343063;
    *((uint32_t*)0x30010004) =  0x38633337;
    *((uint32_t*)0x30010004) =  0x61666135;
    *((uint32_t*)0x30010004) =  0x36343031;
    *((uint32_t*)0x30010004) =  0x39396465;
    *((uint32_t*)0x30010004) =  0x64353964;
    *((uint32_t*)0x30010004) =  0x80000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000000;
    *((uint32_t*)0x30010004) =  0x00000200;
    while ((*((uint32_t*)0x30010000) & 0x4) != 0x4);

    if (*((uint32_t*)0x30010010) != 0x41812DC6) goto fail2;
    if (*((uint32_t*)0x30010014) != 0x0561798D) goto fail2;
    if (*((uint32_t*)0x30010018) != 0xC0CC6E57) goto fail2;
    if (*((uint32_t*)0x3001001C) != 0x4B641893) goto fail2;
    if (*((uint32_t*)0x30010020) != 0xECF4186D) goto fail2;
    if (*((uint32_t*)0x30010024) != 0x4097283A) goto fail2;
    if (*((uint32_t*)0x30010028) != 0xF4A6CFF3) goto fail2;
    if (*((uint32_t*)0x3001002C) != 0xEDDAA4A0) goto fail2;
    goto pass2;

    fail2:
    printstr("Res: FAIL\n");
    printhex(*((uint32_t*)0x30010010), true);
    printhex(*((uint32_t*)0x30010014), true);
    printhex(*((uint32_t*)0x30010018), true);
    printhex(*((uint32_t*)0x3001001C), true);
    printhex(*((uint32_t*)0x30010020), true);
    printhex(*((uint32_t*)0x30010024), true);
    printhex(*((uint32_t*)0x30010028), true);
    printhex(*((uint32_t*)0x3001002C), true);
    goto done2;

    pass2:
    printstr("Res: PASS\n");

    done2:
    // Print identifier
    printstr("Res: ");
    printstr((char*)0x30010050);
    printstr("\n");

    while(1);
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
    reg_mprj_io_32 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_33 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_34 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_35 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_36 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;
    reg_mprj_io_37 = GPIO_MODE_MGMT_STD_INPUT_NOPULL;

}

