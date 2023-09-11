#include "read.hpp"
#include "write.hpp"


inline void test1() {
    pinMode(40, OUTPUT); 
    // delay test
    while (1) {
        // Serial.printf("%u %u\n", decode_32_16(ain0>>1), decode_32_16(ain0));
        // delay(500);
        auto x = read_ain0();
        digitalWriteFast(40, x > 31500);
        write(0, x);
    }
}

void test2() {
    // write speed 37.7Hz x 65536 = 2.5MHz
    while (1) {
        for (int x = 0; x < 65535; ++x) {
            async_write(0, x); 
        }
    }
}

/**
 * @brief Test correctness and crosstalk of ADC
 *
 * Setup:
 *
 * 1. connect 0.25Hz 10Vpp square wave to one of the ADC channel (AIN0, say);
 * 2. observe if the corresponding channel (AIN1) has different results;
 * 3. if the results differ between alternating states, increase DBT of SPI CCR register.
 */
// void test3() {
//     delay(1000);
//     Serial.printf("%u %u %u %u\n", read_ain0(), read_bin0(),read_ain1(), read_bin1());
// }

/**
 * @brief Bandwidth as a unit follower
 *
 */
void test4() {
    while (1) {
        auto x = read_ain0();
        write(0, x);
        read_bin0();
        write(1, x);
        read_bin0();
        write(2, x);
    }
}

