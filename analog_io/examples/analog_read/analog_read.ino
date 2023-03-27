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

#include "analog_io.h"

void setup() {
    while (!Serial)
        ;
    Serial.begin(9600);

    // initialize the chips
    init_chips();
}

void loop() {
    Serial.printf("ain0: %u ain1: %u bin0: %u bin1: %u\n",
                  analogio_read(AIN0),
                  analogio_read(AIN1),
                  analogio_read(BIN0),
                  analogio_read(BIN1));
}
