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


void loop()
{
  if (Serial.available())
  {
    char bytes[3];
    Serial.readBytes(bytes,3);
    
    write(*bytes, *(uint16_t *)(bytes + 1)); 
  }
}
