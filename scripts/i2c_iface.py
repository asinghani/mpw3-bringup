import time
import board
import busio

#i2c = busio.I2C(board.GP3, board.GP2)
i2c = busio.I2C(board.SCL, board.SDA)
assert i2c.try_lock()

TPL0102_ADDR = 0b1010000

MCP1_ADDR = 0b0100001
MCP2_ADDR = 0b0100010
MCP3_ADDR = 0b0100100

DS1077_ADDR = 0b1011000

def i2c_writemulti(addr, reg, data):
    i2c.writeto(addr, bytearray([reg] + data))

def i2c_readmulti(addr, reg, num):
    tmp = bytearray(num)
    i2c.writeto_then_readfrom(addr, bytearray([reg]), tmp)
    return tmp

def i2c_write(addr, reg, data):
    i2c.writeto(addr, bytearray([reg, data]))

def i2c_read(addr, reg):
    tmp = bytearray(1)
    i2c.writeto_then_readfrom(addr, bytearray([reg]), tmp)
    return tmp[0]



def tpl_get_status():
    return i2c_read(TPL0102_ADDR, 0x10)

def tpl_getA():
    return i2c_read(TPL0102_ADDR, 0x00)

def tpl_setA(val):
    i2c_write(TPL0102_ADDR, 0x00, val)

def tpl_getB():
    return i2c_read(TPL0102_ADDR, 0x01)

def tpl_setB(val):
    i2c_write(TPL0102_ADDR, 0x01, val)



def ds1077_setdiv(div):
    assert div >= 2 and div <= 1024
    div = div - 2
    msb = (div >> 2) & 0xFF
    lsb = (div & 0x3) << 6
    i2c_writemulti(DS1077_ADDR, 0x01, [msb, lsb])

def ds1077_getdiv():
    msb, lsb = i2c_readmulti(DS1077_ADDR, 0x01, 2)
    div = (msb << 2) | (lsb >> 6)
    div = div + 2
    return div



def mcp_setup(addr):
    # enable pullups on all pins
    i2c_write(addr, 0x0C, 0xFF)
    i2c_write(addr, 0x0D, 0xFF)

def mcp_read(addr):
    gp = {}

    gpa = i2c_read(addr, 0x12)
    gp.update({f"GPA{i}": (gpa & (1 << i) != 0) for i in range(8)})

    gpb = i2c_read(addr, 0x13)
    gp.update({f"GPB{i}": (gpb & (1 << i) != 0) for i in range(8)})

    return gp

def mcp_setupall():
    mcp_setup(MCP1_ADDR)
    mcp_setup(MCP2_ADDR)
    mcp_setup(MCP3_ADDR)

def mcp_readall():
    gp1 = mcp_read(MCP1_ADDR)
    gp2 = mcp_read(MCP2_ADDR)
    gp3 = mcp_read(MCP3_ADDR)

    return [
        gp2["GPB6"], #  0
        gp2["GPB5"], #  1
        gp2["GPB4"], #  2
        gp2["GPB2"], #  3
        gp2["GPB3"], #  4

        gp1["GPB6"], #  5
        gp1["GPB5"], #  6
        gp1["GPB4"], #  7
        gp1["GPB3"], #  8
        gp1["GPB2"], #  9
        gp1["GPB1"], # 10
        gp1["GPB0"], # 11

        gp1["GPA6"], # 12
        gp1["GPA5"], # 13
        gp1["GPA4"], # 14
        gp1["GPA3"], # 15
        gp1["GPA2"], # 16
        gp1["GPA1"], # 17

        gp3["GPA6"], # 18
        gp3["GPA5"], # 19
        gp3["GPA4"], # 20
        gp3["GPA3"], # 21
        gp3["GPA2"], # 22
        gp3["GPA1"], # 23
        gp3["GPA0"], # 24

        gp3["GPB6"], # 25
        gp3["GPB5"], # 26
        gp3["GPB4"], # 27
        gp3["GPB3"], # 28
        gp3["GPB2"], # 29
        gp3["GPB1"], # 30
        gp3["GPB0"], # 31

        gp2["GPA5"], # 32
        gp2["GPA4"], # 33
        gp2["GPA3"], # 34
        gp2["GPA2"], # 35
        gp2["GPA1"], # 36
        gp2["GPA0"], # 37
    ]

def mcp_readall_indexed():
    return {i: int(x) for i, x in enumerate(mcp_readall())}




