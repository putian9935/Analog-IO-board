import serial

ser = serial.Serial("COM4", baudrate=200000)

from time import sleep 
<<<<<<< Updated upstream
while True:
    if ser.in_waiting:
        print(ser.readline())
        # print(ser.readline())
        # print(ser.readline())
    else:
        sleep(0.01)
    input()
=======
from tqdm import tqdm 
data = []

# while True:
# for _ in tqdm(range(20)):
for _ in tqdm(range(0, 30000, 200)):
    ser.write(int(_).to_bytes(2, 'little', signed=True))
    while not ser.in_waiting:
        sleep(.01)
    else:
        # pass
        print(ser.readline())
        # data.append(int(ser.readline()))
    sleep(0.5)
    # input()

data = data[1:]
print(data)
import numpy as np 
print(np.mean(data), np.std(data))
>>>>>>> Stashed changes
