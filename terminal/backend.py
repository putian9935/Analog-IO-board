from ports import setup_arduino_port
import time 
import struct

ser = setup_arduino_port('COM14')

def sweep(lower, upper):
    ser.write(struct.pack("<BHH", 1, lower, upper))
    while not ser.in_waiting:
        time.sleep(.5)
    while ser.in_waiting:
        print(ser.readline())

def servo(fi, g, wfm):
    ser.write(struct.pack("<dd", fi, g))
    ser.write(wfm)


if __name__ == '__main__':
    print(sweep(2, 1500))