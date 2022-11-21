from ports import setup_arduino_port
import time 
import struct

ser = setup_arduino_port('COM3')

def readback():
    while not ser.in_waiting:
        time.sleep(.5)
    while ser.in_waiting:
        print(ser.readline())

def sweep(ch, lower, upper):
    ser.write(struct.pack("<BBHH", 1, ch, lower, upper))
    readback()

def servo(ch, fi, g, wfm):
    ser.write(struct.pack("<BBdd", 2, ch, fi, g))
    ser.write(wfm)
    readback()

def channel(ch, on):
    ser.write(struct.pack("<BB", 3, ch + (1 << 7) * on))
    readback()

def stop(args):
    ser.write(b'\x00')
    
if __name__ == '__main__':
    print(sweep(2, 1500))