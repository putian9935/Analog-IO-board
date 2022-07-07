#include "clockspeed.h"
#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"

void setup()
{
    // This will set a higher IPG root clock
    CCM_CBCDR = (CCM_CBCDR & ~CCM_CBCDR_IPG_PODF_MASK) | CCM_CBCDR_IPG_PODF(1);
    // while (!Serial)
    // ;
    // Serial.begin(115200);
    set_arm_clock_cpp(800000000);

    init_chips();
}

void loop()
{
    // for(int16_t i = -16; i < 17; ++i) {
    //   write(0, i);
    //   delayMicroseconds(1);
    // }
    // for(int16_t i = 16; i > -16; --i) {
    //   write(0, i);
    //   delayMicroseconds(1);
    // }

    #define INNER_RANGE 280
    for (int16_t i = -6000; i < 5501; i += 32)
    {
        // write(0, i);
        // delayMicroseconds(30);
        for (int16_t j = -INNER_RANGE; j < INNER_RANGE; j+=20)
        {
            write(0, i + j); // current DC mod
            delayNanoseconds(400);
            // delayMicroseconds(1);
            write(3, j * 100); // LO
            delayNanoseconds(400);
        }
        for (int16_t j = INNER_RANGE; j > -INNER_RANGE; j-=20)
        {
            write(0, i + j);delayNanoseconds(400);
            // delayMicroseconds(1);
            write(3, j * 100);delayNanoseconds(400);
        }
    }
    for (int16_t i = 5500; i > -6001; i -= 32)
    {
        for (int16_t j = -INNER_RANGE; j < INNER_RANGE; j+=20)
        {
            write(0, i + j);delayNanoseconds(400);
            // delayMicroseconds(1);
            write(3, j * 100);delayNanoseconds(400);
        }
        for (int16_t j = INNER_RANGE; j > -INNER_RANGE; j-=20)
        {
            write(0, i + j);delayNanoseconds(400);
            // delayMicroseconds(1);
            write(3, j* 100);delayNanoseconds(400);
        }
    }

    // static union
    // {
    //     struct
    //     {
    //         int16_t value;
    //         int8_t channel;
    //     };
    //     char bytes[3];
    // } update;

    // if (Serial.available())
    // {
    //     Serial.readBytes(update.bytes, 3);
    //     Serial.println(ain0);
    // }

    // for (int16_t j = -16; j < 17; ++j)
    // {
    //     write(0, update.value + j);
    //     delayNanoseconds(400);
    //     write(3, j * 100);
    //     delayNanoseconds(400);
    // }
    // for (int16_t j = 16; j > -16; --j)
    // {
    //     write(0, update.value + j);
    //     delayNanoseconds(400);
    //     write(3, j * 100);
    //     delayNanoseconds(400);
    // }
}
