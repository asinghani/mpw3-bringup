# number of IO in the configuration stream for each chain
NUM_IO = 19

# defines these values for IO configurations
C_MGMT_OUT = 0
C_MGMT_IN = 1
C_USER_BIDIR = 2
C_DISABLE = 3
C_ALL_ONES = 4
C_USER_BIDIR_WPU = 5
C_USER_BIDIR_WPD = 6
C_USER_IN_NOPULL = 7
C_USER_OUT = 8

"""
PONG

.VGA_R3(io_out[19]),
.VGA_R2(io_out[18]),
.VGA_R1(io_out[17]), -- UNUSED
.VGA_R0(io_out[16]), -- UNUSED

.VGA_G3(io_out[15]),
.VGA_G2(io_out[14]),
.VGA_G1(io_out[13]), -- UNUSED
.VGA_G0(io_out[12]), -- UNUSED

.VGA_B3(io_out[11]),
.VGA_B2(io_out[10]),
.VGA_B1(io_out[9]),  -- UNUSED
.VGA_B0(io_out[8]),  -- UNUSED

.VGA_VS(io_out[20]),
.VGA_HS(io_out[21]),

.btn_serve(io_in[27]),
.btn_rst(io_in[26]),
.btn0_n(~io_in[25]),
.btn1_n(~io_in[24]),
.btn2_n(~io_in[23]),
.btn3_n(~io_in[22]),

"""


config_h = [
    C_MGMT_OUT,  #37
    C_MGMT_OUT,  #36
    C_MGMT_OUT,  #35
    C_MGMT_OUT,  #34
    C_MGMT_OUT,  #33
    C_USER_OUT,  #32
    C_USER_OUT,  #31
    C_USER_OUT,  #30
    C_USER_IN_NOPULL,  #29
    C_USER_IN_NOPULL,  #28
    C_USER_IN_NOPULL,  #27
    C_USER_IN_NOPULL,  #26
    C_USER_IN_NOPULL,  #25
    C_USER_IN_NOPULL,  #24
    C_USER_IN_NOPULL,  #23
    C_USER_IN_NOPULL,  #22
    C_USER_OUT,  #21
    C_USER_OUT,  #20
    C_USER_OUT,  #19
]

del config_h[NUM_IO:]

config_l = [
    C_MGMT_OUT,   #0
    C_MGMT_OUT,   #1
    C_MGMT_OUT,   #2
    C_MGMT_OUT,   #3
    C_MGMT_OUT,   #4
    C_MGMT_OUT,   #5
    C_MGMT_OUT,   #6
    C_MGMT_OUT,   #7
    C_MGMT_OUT,   #8
    C_MGMT_OUT,   #9
    C_USER_OUT,   #10
    C_USER_OUT,   #11
    C_MGMT_OUT,   #12
    C_MGMT_OUT,   #13
    C_USER_OUT,   #14
    C_USER_OUT,   #15
    C_MGMT_OUT,   #16
    C_MGMT_OUT,   #17
    C_USER_OUT,   #18
]

del config_l[NUM_IO:]
