#include "clockspeed.h"
#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"

void setup()
{
    // This will set a higher IPG root clock
    CCM_CBCDR = (CCM_CBCDR & ~CCM_CBCDR_IPG_PODF_MASK) | CCM_CBCDR_IPG_PODF(1);
    set_arm_clock_cpp(800000000);
    init_chips();
}

void loop()
{
}
