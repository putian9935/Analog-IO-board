/**
 * @file utilities.hpp
 * @brief Useful utilities
 * @date 2022-11-28
 * 
 */
#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>

#define ASSERT_ADC digitalWriteFast(CS, LOW)
#define DEASSERT_ADC digitalWriteFast(CS, HIGH)

#define ASSERT_DAC1 digitalWriteFast(DAC_SYNC1, LOW)
#define DEASSERT_DAC1 digitalWriteFast(DAC_SYNC1, HIGH)

#define ASSERT_DAC2 digitalWriteFast(DAC_SYNC2, LOW)
#define DEASSERT_DAC2 digitalWriteFast(DAC_SYNC2, HIGH)

extern uint32_t set_arm_clock_cpp(uint32_t); 

#define ADC_TRANSFER_NOP ASSERT_ADC;transfer16(ADC_NOP);DEASSERT_ADC

/**
 * @brief Reset ADC 
 * 
 * @note this function must be called before end_fourway
 */
void adc_reset();


/** GPIO driver setup for better SI at high frequencies. See section 12.3.1 on page 949  */
#define FAST_IO IOMUXC_PAD_DSE(4) | IOMUXC_PAD_SPEED(3) | IOMUXC_PAD_SRE

void set_fastio_pin(uint8_t pin_num); 

/**
 * @brief Calibrate DAC channel ch with offset and fgain 
 * 
 * see section "OFFSET AND GAIN ADJUSTMENT WORKED EXAMPLE" of the datasheet for how to calculate parameters
 * @note this function must be called *after* init_chips
 */
void calibrate_dac(uint8_t ch, uint8_t offset, uint8_t fgain); 

#endif