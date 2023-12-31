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


void runTest(uint32_t key0, uint32_t key1, uint32_t key2, uint32_t key3,
             uint32_t key4, uint32_t key5, uint32_t key6, uint32_t key7,
             uint32_t iv0, uint32_t iv1, uint32_t iv2, uint32_t iv3,
             uint32_t in0, uint32_t in1, uint32_t in2, uint32_t in3,
             uint32_t out0, uint32_t out1, uint32_t out2, uint32_t out3,
             bool verbose, bool enc, bool aes256) {

    uint32_t aes_mask = aes256 ? 0x10 : 0x0;
    *((volatile uint32_t*) 0x30000000) = aes_mask | 0;

    if (aes256) {
        *((volatile uint32_t*) 0x30000040) = key4;
        *((volatile uint32_t*) 0x30000040) = key5;
        *((volatile uint32_t*) 0x30000040) = key6;
        *((volatile uint32_t*) 0x30000040) = key7;
        *((volatile uint32_t*) 0x3000000C) = 2;
        if (verbose) printstr("tkey\n");
        while((*((volatile uint32_t*) 0x30000000) & 0x3) != 0x3);
    }

    // Load key
    *((volatile uint32_t*) 0x30000040) = key0;
    *((volatile uint32_t*) 0x30000040) = key1;
    *((volatile uint32_t*) 0x30000040) = key2;
    *((volatile uint32_t*) 0x30000040) = key3;

    // Update key
    *((volatile uint32_t*) 0x3000000C) = 1;
    if (verbose) printstr("key\n");
    while((*((volatile uint32_t*) 0x30000000) & 0x3) != 0x3);

    // Set IV
    if (iv0 | iv1 | iv2 | iv3) {
        *((volatile uint32_t*) 0x30000000) = aes_mask | 0;
        *((volatile uint32_t*) 0x30000010) = iv0;
        *((volatile uint32_t*) 0x30000010) = iv1;
        *((volatile uint32_t*) 0x30000010) = iv2;
        *((volatile uint32_t*) 0x30000010) = iv3;
        *((volatile uint32_t*) 0x30000008) = 1;

        while((*((volatile uint32_t*) 0x30000000) & 0x4) != 0x4);
        *((volatile uint32_t*) 0x30000000) = aes_mask | 8;
    } else {
        *((volatile uint32_t*) 0x30000000) = aes_mask | 0;
    }
    if (verbose) printstr("iv\n");

    // Load input data
    *((volatile uint32_t*) 0x30000010) = in0;
    *((volatile uint32_t*) 0x30000010) = in1;
    *((volatile uint32_t*) 0x30000010) = in2;
    *((volatile uint32_t*) 0x30000010) = in3;

    if (enc) *((volatile uint32_t*) 0x30000004) = 1;
    else *((volatile uint32_t*) 0x30000008) = 1;

    if (verbose) printstr("data\n");

    while((*((volatile uint32_t*) 0x30000000) & 0x4) != 0x4);

    // Read back result
    if (*((volatile uint32_t*) 0x30000030) == out0 &&
        *((volatile uint32_t*) 0x30000034) == out1 &&
        *((volatile uint32_t*) 0x30000038) == out2 &&
        *((volatile uint32_t*) 0x3000003C) == out3) {
        printstr("Res: PASS\n");
    } else {
        printstr("Res: FAIL\n");
        verbose = 1;
    }

    if (verbose) {
        printhex(*((volatile uint32_t*) 0x30000030), true);
        printhex(*((volatile uint32_t*) 0x30000034), true);
        printhex(*((volatile uint32_t*) 0x30000038), true);
        printhex(*((volatile uint32_t*) 0x3000003C), true);
        printstr("\n");
    }

}

void main()
{
    setup_test();

    printstr("aes\n");

    bool verbose = 0;

    // Load top key as 165406623308029099331681619155339851573
    // Load key as 105658700701783706739513152549528565804
    // Set IV to 98873892024534219918523500662347411235
    // Decrypt 124392828678387920155199777039481918860
    // Expect 67096222749627335679586640564596122443
    printstr("de1\n");
    runTest(1333600292, 2067428908, 724464948, 1210611756,
            2087725097, 2049403466, 1044015937, 1296971573,
            1247963967, 1903436343, 2034596968, 1952329507,
            1570058230, 4070621038, 2773892934, 3961869708,
            846873392, 1043276127, 961370419, 674374475,
            verbose, 0, 1);

    // Load top key as 165406623308029099331681619155339851573
    // Load key as 105658700701783706739513152549528565804
    // Set IV to 98873892024534219918523500662347411235
    // encrypt 67096222749627335679586640564596122443
    // Expect 124392828678387920155199777039481918860
    printstr("en1\n");
    runTest(1333600292, 2067428908, 724464948, 1210611756,
            2087725097, 2049403466, 1044015937, 1296971573,
            1247963967, 1903436343, 2034596968, 1952329507,
            846873392, 1043276127, 961370419, 674374475,
            1570058230, 4070621038, 2773892934, 3961869708,
            verbose, 1, 1);

    // Load key as 114849718896073566083416993308083054187
    // Set IV to 94605682877008349257125771496548550738
    // Encrypt 54758945827980741386470298169332301409
    // Expect 63241459423936053969797744104158954336
    printstr("en2\n");
    runTest(1449607251, 1734159223, 1713582630, 908945003,
            0, 0, 0, 0,
            1194091594, 577847158, 1245724226, 1512654930,
            691155065, 845884219, 1382968177, 1333362273,
            798219438, 3687767725, 1414612439, 2930934624,
            verbose, 1, 0);

    // Load key as 114849718896073566083416993308083054187
    // Set IV to 94605682877008349257125771496548550738
    // Decrypt 63241459423936053969797744104158954336
    // Expect 54758945827980741386470298169332301409
    printstr("de2\n");
    runTest(1449607251, 1734159223, 1713582630, 908945003,
            0, 0, 0, 0,
            1194091594, 577847158, 1245724226, 1512654930,
            798219438, 3687767725, 1414612439, 2930934624,
            691155065, 845884219, 1382968177, 1333362273,
            verbose, 0, 0);

    printstr("Res: ");
    printstr((char*)0x30000050);
    printstr("\n");

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

