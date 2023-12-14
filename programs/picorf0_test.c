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

    printstr("init\n");
    
    reg_la_sample = 1;
    reg_la_sample = 0;
    printhex(reg_la0_data_in, true);
    printhex(reg_la1_data_in, true);
    printhex(reg_la2_data_in, true);
    printhex(reg_la3_data_in, true);

    printhex(reg_la0_data, true);
    printhex(reg_la1_data, true);
    printhex(reg_la2_data, true);
    printhex(reg_la3_data, true);

    printstr("start picorF0\n");

    // Wait for instr read
    uint16_t buf[256];
    while (true) {
        while (!(reg_la1_data_in & 0x00030000)) {
            if (reg_la1_data_in & 0x00100000) {
                printstr("halted\n");
                while(1);
            }
        }

        if (reg_la1_data_in & 0x00010000) { // IF
            uint32_t pc = reg_la0_data_in & 0x0000FFFF;
            printstr("I");
            printhex(pc, true);

            reg_la2_data = (get_prog(pc) << 16) | get_prog(pc + 2);
            reg_la3_data = 0x3; // STB, ACK
            reg_la3_data = 0x0; // no STB, ACK
            

        } else if (reg_la1_data_in & 0x00040000) { // DWrite
            uint32_t adr = reg_la0_data_in >> 16;
            uint32_t dat = reg_la1_data_in & 0x0000FFFF;
            printstr("DW");
            printhex(adr, false);
            printstr("=");
            printhex(dat, true);
            buf[adr & 0xFF] = dat;
            reg_la3_data = 0xC; // STB, ACK
            reg_la3_data = 0x0; // no STB, ACK
        
        } else { // DRead
            uint32_t adr = reg_la0_data_in >> 16;
            printstr("DR");
            printhex(adr, true);
            reg_la2_data = buf[adr & 0xFF];
            reg_la3_data = 0xC; // STB, ACK
            reg_la3_data = 0x0; // no STB, ACK
        }
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


uint32_t get_prog(uint32_t pc) {
    switch (pc) {
        case 0:    return 0xF800;
        case 2:    return 0x0020;
        case 4:    return 0x31C0;
        case 6:    return 0xABCD;
        case 8:    return 0x3807;
        case 10:   return 0x9000;
        case 12:   return 0x3800;
        case 14:   return 0x9002;
        case 16:   return 0xFE00;
        case 18:   return 0x31C0;
        case 20:   return 0x0001;
        case 22:   return 0x3807;
        case 24:   return 0x9000;
        case 26:   return 0x3801;
        case 28:   return 0x9002;
        case 30:   return 0xFE00;
        case 32:   return 0x0000;
        case 34:   return 0x3040;
        case 36:   return 0x0001;
        case 38:   return 0x3100;
        case 40:   return 0x8000;
        case 42:   return 0x3140;
        case 44:   return 0x7FFF;
        case 46:   return 0x00E5;
        case 48:   return 0x3080;
        case 50:   return 0xFFFF;
        case 52:   return 0x1013;
        case 54:   return 0xC800;
        case 56:   return 0x003E;
        case 58:   return 0xF800;
        case 60:   return 0x0012;
        case 62:   return 0x2000;
        case 64:   return 0x3040;
        case 66:   return 0x0002;
        case 68:   return 0x3100;
        case 70:   return 0x8000;
        case 72:   return 0x3140;
        case 74:   return 0x7FFF;
        case 76:   return 0x00E5;
        case 78:   return 0xC800;
        case 80:   return 0x0012;
        case 82:   return 0xF800;
        case 84:   return 0x0056;
        case 86:   return 0x2000;
        case 88:   return 0x3040;
        case 90:   return 0x0003;
        case 92:   return 0x3100;
        case 94:   return 0x8000;
        case 96:   return 0x3140;
        case 98:   return 0x7FFF;
        case 100:  return 0x00E5;
        case 102:  return 0xA800;
        case 104:  return 0x0012;
        case 106:  return 0xF800;
        case 108:  return 0x006E;
        case 110:  return 0x2000;
        case 112:  return 0x3040;
        case 114:  return 0x0004;
        case 116:  return 0x3100;
        case 118:  return 0x8000;
        case 120:  return 0x3140;
        case 122:  return 0x7FFF;
        case 124:  return 0x00E5;
        case 126:  return 0x9800;
        case 128:  return 0x0086;
        case 130:  return 0xF800;
        case 132:  return 0x0012;
        case 134:  return 0x2000;
        case 136:  return 0x3040;
        case 138:  return 0x0005;
        case 140:  return 0x3100;
        case 142:  return 0x8000;
        case 144:  return 0x3140;
        case 146:  return 0x7FFF;
        case 148:  return 0x00E5;
        case 150:  return 0xB800;
        case 152:  return 0x0012;
        case 154:  return 0xF800;
        case 156:  return 0x009E;
        case 158:  return 0x2000;
        case 160:  return 0x3040;
        case 162:  return 0x0006;
        case 164:  return 0x3100;
        case 166:  return 0x8000;
        case 168:  return 0xF2E0;
        case 170:  return 0x000F;
        case 172:  return 0x3080;
        case 174:  return 0xFFFF;
        case 176:  return 0x1013;
        case 178:  return 0xC800;
        case 180:  return 0x00BA;
        case 182:  return 0xF800;
        case 184:  return 0x0012;
        case 186:  return 0x2000;
        case 188:  return 0x3040;
        case 190:  return 0x0007;
        case 192:  return 0x3100;
        case 194:  return 0x8000;
        case 196:  return 0x3140;
        case 198:  return 0x0000;
        case 200:  return 0x80E0;
        case 202:  return 0x3080;
        case 204:  return 0x7FFF;
        case 206:  return 0x1013;
        case 208:  return 0xC800;
        case 210:  return 0x00D8;
        case 212:  return 0xF800;
        case 214:  return 0x0012;
        case 216:  return 0x2000;
        case 218:  return 0x3040;
        case 220:  return 0x0008;
        case 222:  return 0x30C0;
        case 224:  return 0xABCD;
        case 226:  return 0x3080;
        case 228:  return 0xA000;
        case 230:  return 0x3813;
        case 232:  return 0x0323;
        case 234:  return 0x3100;
        case 236:  return 0x0323;
        case 238:  return 0x2920;
        case 240:  return 0xA000;
        case 242:  return 0x5220;
        case 244:  return 0xABCD;
        case 246:  return 0xC800;
        case 248:  return 0x00FE;
        case 250:  return 0xF800;
        case 252:  return 0x0012;
        case 254:  return 0x2000;
        case 256:  return 0xF800;
        case 258:  return 0x0004;
        default:   return 0x0000;
    }
}


