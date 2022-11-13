#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"


void setup()
{
    while (!Serial)
        ;
    Serial.begin(115200);
    init_chips();
    pinMode(4, OUTPUT);
}

void loop()
{
    // uint16_t y = read_ain0();
    // volatile uint16_t x; 
    // x = 20000;
    // digitalWriteFast(4, HIGH);
    
    // write(4, x);
    // delay(1);
    
    // x = 1000;
    // digitalWriteFast(4, LOW);
    // write(4, x);
    // delay(1);
    if(read_ain0() > 40000) {
        digitalWriteFast(4, HIGH);
        write(4, 25000);
    }
    else {
        digitalWriteFast(4, LOW);
        write(4,9000);
    }
    // write(4, read_ain0());
}
