import serial

ser = serial.Serial("COM20", baudrate=200000)

from time import sleep 
while True:
    ser.write(b'?')
    if ser.in_waiting:
        print(ser.readline())
        # print(ser.readline())
        # print(ser.readline())
    else:
        sleep(0.01)
    input()
