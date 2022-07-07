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

#define MAXN 500 

void loop()
{
  if (Serial.available())
  {
    static uint16_t res[MAXN];
    // char bytes[3];
    // Serial.readBytes(bytes,3);
    
    // write(*bytes, *(uint16_t *)(bytes + 1)); 
    for(int i = 0; i < MAXN; ++i)
    {
        res[i] = ain0; 
        delayMicroseconds(1);
    }
    for(int i = 0; i < MAXN; ++i)
        Serial.printf("%d ", res[i]);
    Serial.printf("\n");

  }
}
