#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"
#include "simulator.cpp"

extern void adc_init(); 

void setup()
{
    while (!Serial)
        ;
    Serial.begin(115200);
    init_chips();
    // adc_init();
}

extern void adc_test(); 
void loop()
{
    // adc_test();
    // delay(1);
    write(0, 0);
    delay(1);
    write(0, 10000);
    delay(1);
    // write(5, 65535);
    // delayMicroseconds(20);
    // write(5, 0);
    // delayMicroseconds(10);
    // write(5, 2);
    // delayMicroseconds(10);
    // write(5, 4);
    // delayMicroseconds(10);
    // write(5, 8);
    // delayMicroseconds(10);
    // write(5, 16);
    // delayMicroseconds(10);

}
