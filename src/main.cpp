#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"

extern void adc_init(); 

void setup()
{
    while (!Serial)
        ;
    Serial.begin(115200);
    init_chips();
    adc_init();
}

extern uint16_t adc_test(); 
void loop()
{
    auto x = adc_test();
    write(4, x);
}
