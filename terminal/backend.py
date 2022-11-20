from ports import setup_arduino_port
import time 
import struct
from ast import literal_eval

ser = setup_arduino_port('COM3')

def sweep(lower, upper):
    ser.write(struct.pack("<BHH", 1, lower, upper))
    while not ser.in_waiting:
        time.sleep(.5)
    while ser.in_waiting:
        print(ser.readline())

def servo(fi, g, wfm):
    ser.write(struct.pack("<Bdd", 2, eval(fi.strip('"')), g))
    ser.write(wfm)
    while not ser.in_waiting:
        time.sleep(.5)
    while ser.in_waiting:
        print(ser.readline())

def stop(args):
    ser.write(b'\x00')
    
if __name__ == '__main__':
    print(sweep(2, 1500))