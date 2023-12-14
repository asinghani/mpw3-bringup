# MPW3 Bringup / Characterization

Experiments in bringing up SKY130 MPW-3 chips from the initial tapeout in November 2021. Similarly to [MPW-1](https://github.com/asinghani/mpw1-bringup), the standardized "Caravel" padframe used in this tapeout had hold-time violations between 

## Breakout PCBs

- Characterization PCB [Schematic](boards/mpw3-common/schematic.pdf)
    - Independent voltage regulators for padframe and core (with non-volatile memory)
    - DS1077LU-50 clock generator with 50kHz - 50MHz output frequency + non-volatile memory
    - MCP23017 GPIO expanders for scanning chip I/Os
    - MCP1319 reset supervisor
    - MicroMod-style socket for Caravel chip breakout

- FTDI Breakout [Schematic](boards/uart-breakout/schematic.pdf)
    - Connects characterization PCB to FT232RL for UART access over USB
    - Includes LEDs, switches, and SPI FRAM for use with picoRF0 CPU

- VGA Breakout [Schematic](boards/vga-breakout/schematic.pdf)
    - Breaks out infinite-runner game and pong game to a 2-bit VGA DAC
    - Includes buffer chips for 

## Repo Structure

- `boards/` - KiCAD design files for breakout PCBs
- `parts/` - I/O characterization results for several dies
- `scripts/i2c_iface.py` - Script for interfacing with the voltage regulators, clock generator, and GPIO expanders on the characterization board
- `programs/` - Programs for running on the Caravel processor for the different applications
    - `aes_test.c` - Test the AES core on crypto-accelerator-chip-v2 design
    - `gpio_config_io_dino.py` - GPIO configuration for the VGA runner game
    - `gpio_config_io_pong.py` - GPIO configuration for the VGA pong game
    - `picorf0_test.c` - Test program for the picoRF0 custom CPU core
    - `read_ident.c` - Reads the ROM identifier and build number in the SHA and AES cores
    - `sha256_test.c` - Test the SHA256 core with some test vectors
    - `spi_fram.c` - Test program for using the Caravel SPI master to interface with an FRAM chip
    - `uart_working.c` - Workaround for UART RX on MPW-3 Caravel
