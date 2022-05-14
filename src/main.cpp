#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"


void setup()
{
    // This will set a higher IPG root clock 
    CCM_CBCDR = (CCM_CBCDR & ~CCM_CBCDR_IPG_PODF_MASK) | CCM_CBCDR_IPG_PODF(1);
    while (!Serial);
    Serial.begin(115200);
    
    
    init_chips();
}

elapsedMicros t;
void loop()
{
    int cnt = 0;
    t = 0;
    while (t < 1000000)
    {// write(2, ain0 >> 3);
    write(2, 0x55);
    write(7, 0x52);
    ++cnt;
    }
    Serial.println(cnt);
    // Serial.printf("%u %u %u %u\n", ain0, ain1, bin0, bin1);
}


