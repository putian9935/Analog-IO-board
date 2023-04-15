from ports import setup_arduino_port
import time
import struct

ser = setup_arduino_port('COM7')

def readback():
    while not ser.in_waiting:
        time.sleep(.1)
    while ser.in_waiting:
        print(ser.readline())

def readback_val():
    mess_buffer = []
    while not ser.in_waiting:
        time.sleep(.1)
    while ser.in_waiting:
        mess_buffer += [ser.readline()]
    return mess_buffer

def sweep(ch, lower, upper):
    ser.write(struct.pack("<BBHH", 1, ch, lower, upper))
    readback()

def sweep_r(ch, lower, upper):
    ser.write(struct.pack("<BBHH", 1, ch, lower, upper))
    return readback_val()


def servo(ch, fi, g, wfm):
    ser.write(struct.pack("<BBdd", 2, ch, fi, g))
    ser.write(wfm)
    readback()

def channel(ch, on):
    ser.write(struct.pack("<BB", 3, ch + (1 << 7) * on))
    readback()

def hsp(*sp):
    ser.write(struct.pack("<BHHHH", 4, *sp))
    readback()
    channel(3, True) 

def show(ch):
    ser.write(struct.pack("<BB", 5, ch)) 
    readback()
    
def stop(args):
    ser.write(b'\x00')

if __name__ == '__main__':
    print(sweep(2, 1500))
