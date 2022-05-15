#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"

void setup()
{
    // This will set a higher IPG root clock
    CCM_CBCDR = (CCM_CBCDR & ~CCM_CBCDR_IPG_PODF_MASK) | CCM_CBCDR_IPG_PODF(1);
    while (!Serial)
        ;
    Serial.begin(115200);

    init_chips();
}

elapsedMicros t;
void loop()
{
    int cnt = 0;
    t = 0;
    while (t < 1000000)
    {
        for (int i = 0; i < 90; ++i)
        {
            write(2, i << 6);
        }
        ++cnt;
    }
    Serial.println(cnt);
}
