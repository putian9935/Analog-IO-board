import serial

ser = serial.Serial("COM20", baudrate=200000)

# ser.write(b'?')

from time import sleep 
while True:
    if ser.in_waiting:
        print(ser.readline())
        # print(ser.readline())
        # print(ser.readline())
    else:
        sleep(0.01)
    input()
print(ser.readline())
print(ser.readline())
print(ser.readline())

ch = [[], [], [], []]

import matplotlib.pyplot as plt 

l = []
labels = ['1.25V REF', 'TEMP SENSOR', '1.10V REF', 'GND']
for cd, ll in zip(ch, labels):
    l.append(plt.plot(cd, label=ll)[0])
    print(type(l[0]))
xs  = []
cnt = 0 
from tqdm import tqdm 
pbar = tqdm()
while True:
    for _ in range(2000):
        xs.append(cnt)
        cnt += 1
        ser.write(b'?')
        for c in range(4):
            ch[c].append(int(ser.readline()) / 65535 * 2.5)
            
    
    for c in range(4):
        l[c].set_xdata(xs)
        l[c].set_ydata(ch[c])
    plt.gca().set_xlim(0, cnt)
    plt.gca().set_ylim(-.2, 1.4)
    plt.legend(loc=3)
    plt.xlabel('Read counts')
    plt.ylabel('Voltage / V')
    pbar.update(2000)
    plt.pause(1e-2)
