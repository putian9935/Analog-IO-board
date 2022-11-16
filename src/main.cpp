#include "clockspeed.h"
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
    // static PIDController pidc(0, 450, 450, 250, 55000); // 256 samples
    static PIDController pidc(0, 450, 150, 50, 55000); // 64 samples
    pidc.reference = 40000;
    for (int i = 0; i < 300; ++i)
    {
        pidc.update();
    }
    pidc.reference = 0;
    for (int i = 0; i < 300; ++i)
    {
        pidc.update();
    }
}
