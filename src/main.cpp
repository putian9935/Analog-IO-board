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

void loop()
{
    // write(4, read_ain0());
    write(4, 0);
    write(4, 1);
}
