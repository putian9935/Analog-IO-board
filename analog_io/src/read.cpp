/**
 * @file read.cpp
 * @brief Interface for getting ADC readings
 * @date 2022-11-28
 *
 */
#include "read.hpp"
#include "bit_mangler.h"

extern volatile uint32_t ain0, ain1 ;

uint16_t read_ain0() {
    return decode_32_16(ain0 >> 1);
}

uint16_t read_bin0() {
    return decode_32_16(ain0);
}

#ifdef ADC_FOUR_CHANNEL
uint16_t read_ain1() {
    return decode_32_16(ain1 >> 1);
}

uint16_t read_bin1() {
    return decode_32_16(ain1);
}
#endif