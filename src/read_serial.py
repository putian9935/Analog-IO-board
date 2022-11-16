import serial

ser = serial.Serial("COM6", baudrate=200000)

ser.set_buffer_size(tx_size = 65536, rx_size=65536)
f = open('from_teensy', 'wb')
from time import sleep 
while True:
    ser.write(b'?')
    for _ in range(5):
        sleep(1)
        if ser.in_waiting:
            print(ser.in_waiting)
            line = ser.read(ser.in_waiting)
            f.write(line)
            print(line)
            # print(ser.readline())
            # print(ser.readline())
            # print(ser.readline())
        else:
            sleep(0.01)
        ser.write(b'?')
        
    input()
