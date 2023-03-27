#include "analog_io.h"

#define SERIAL_BAUD_RATE 9600

void setup()
{
    init_chips();   
    Serial.begin(SERIAL_BAUD_RATE);
}

elapsedMicros t; 
void loop()
{
    int cnt = 0; 
    t = 0; 
    while(t < 10000) {
        write(0, 0);
        ++cnt; 
    }
    Serial.println(cnt);
}
