#include "clockspeed.h"
#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"

void setup()
{
    // This will set a higher IPG root clock
    CCM_CBCDR = (CCM_CBCDR & ~CCM_CBCDR_IPG_PODF_MASK) | CCM_CBCDR_IPG_PODF(1);
    // while (!Serial)
    ;
    Serial.begin(115200);
    set_arm_clock_cpp(800000000);

    init_chips();
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
