/**
 * @file analog_read.ino
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-03-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <tuple>  // for std::tie
#include "analog_io.h"

void setup() {
    while (!Serial)
        ;
    Serial.begin(9600);

    // initialize the chips
    init_chips();
}

void loop() {
    // single read
    Serial.printf("ain0: %u ain1: %u bin0: %u bin1: %u\n",
                  analogio_read(AIN0),
                  analogio_read(AIN1),
                  analogio_read(BIN0),
                  analogio_read(BIN1));

    // pair read
    // in case the Teensy-bundled compiler upgrades in the future, 
    // or you may build with your latest arm toolchain
#ifdef __cpp_structured_bindings
    auto [ain0, bin0] = analogio_read_both(CH0);
    auto [ain1, bin1] = analogio_read_both(CH1);
#else
    uint16_t ain0, bin0, ain1, bin1;
    std::tie(ain0, bin0) = analogio_read_both(CH0);
    std::tie(ain1, bin1) = analogio_read_both(CH1);
#endif
    Serial.printf("ain0: %u ain1: %u bin0: %u bin1: %u\n",
                  ain0,
                  ain1,
                  bin0,
                  bin1);
}
