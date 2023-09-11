#include "utilities.hpp"
#include <SPI.h>
#include "bit_mangler.h"
#include "clockspeed.h"
#include "init_chips.hpp"
#include "write.hpp"

extern void fourway_write(uint16_t);

void adc_reset() {
    fourway_write((uint16_t)(ADC_WRITE | ADC_CFG2 | ADC_CFG2_HRST));
    fourway_write(0);
    fourway_write(0);

    delay(1);
}

void set_fastio_pin(uint8_t const pin_num) {
    *(portControlRegister(pin_num)) = FAST_IO;
}
