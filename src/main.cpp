#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"
#include "clockspeed.h"


void setup()
{
    // This will set a higher IPG root clock
    CCM_CBCDR = (CCM_CBCDR & ~CCM_CBCDR_IPG_PODF_MASK) | CCM_CBCDR_IPG_PODF(1);
    while (!Serial)
        ;
    Serial.begin(115200);
    set_arm_clock_cpp(800000000);

    init_chips();
}

elapsedMicros t;
void loop()
{
    // int cnt = 0;
    // t = 0;
    // while (t < 1000000)
    // {
        write_both(2, ain0 >> 3, 5, ain0 >> 3);
        // ++cnt;
    // }
    // Serial.println(cnt);
}
