/**
 * @file digital_rw.ino
 * @author Li Putian
 * @brief Basic use of digital IO.
 * @date 2023-03-29
 *
 */

#include "analog_io.h"
void init() {
    init_chips(); 
    digitalio_mode(DIO1, OUTPUT);
    // by default, a digital pin is in INPUT mode
    // digitalio_mode(DIO2, INPUT);
}

void loop() {
    // connect DIO1 and DIO2 externally, the circuit is a ring oscillator
    digitalio_write(DIO1, ~digitalio_read(DIO2));
}