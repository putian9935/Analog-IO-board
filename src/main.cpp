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
    adc_init();
}

extern void adc_test(); 
void loop()
{
    adc_test();
    delay(1);
}
