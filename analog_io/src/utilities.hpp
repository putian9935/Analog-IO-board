/**
 * @file utilities.hpp
 * @brief Useful utilities
 * @date 2022-09-11
 *
 */
#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>

extern uint32_t set_arm_clock_cpp(uint32_t);

#define ADC_TRANSFER_NOP \
    ASSERT_ADC;          \
    transfer16(ADC_NOP); \
    DEASSERT_ADC

/**
 * @brief Reset ADC
 *
 * @note this function must be called before end_fourway
 */
void adc_reset();

/** GPIO driver setup for better SI at high frequencies. See section 12.3.1 on page 949  */
#define FAST_IO IOMUXC_PAD_DSE(7) | IOMUXC_PAD_SPEED(3) | IOMUXC_PAD_SRE

void set_fastio_pin(uint8_t const pin_num);

#endif