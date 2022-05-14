import serial

ser = serial.Serial("COM4", baudrate=200000)

from time import sleep 
while True:
    if ser.in_waiting:
        print(ser.readline())
        # print(ser.readline())
        # print(ser.readline())
    else:
        sleep(0.01)
    input()
