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

void calibrate_dac(uint8_t const ch, uint8_t const offset, uint8_t const fgain) {
    uint64_t new_dac_num = insert_zeros(((((uint32_t)((ch & 3) | DAC_OFFSET_REG)) << 16) | offset));
    if (ch < 4)
        transfer_dac24(new_dac_num << 1);
    else
        transfer_dac24(new_dac_num);

    new_dac_num = insert_zeros(((((uint32_t)((ch & 3) | DAC_FGAIN_REG)) << 16) | fgain));
    if (ch < 4)
        transfer_dac24(new_dac_num << 1);
    else
        transfer_dac24(new_dac_num);
}
