#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"
#include "simulator.cpp"


void setup()
{
    while (!Serial)
        ;
    Serial.begin(115200);
    init_chips();
}

void loop()
{
    write(5, 65535);
    delayMicroseconds(20);
    write(5, 0);
    delayMicroseconds(10);
    write(5, 2);
    delayMicroseconds(10);
    write(5, 4);
    delayMicroseconds(10);
    write(5, 8);
    delayMicroseconds(10);
    write(5, 16);
    delayMicroseconds(10);

}
