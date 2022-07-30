#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>

#define ASSERT_ADC digitalWriteFast(CS, LOW)
#define DEASSERT_ADC digitalWriteFast(CS, HIGH)

#define ASSERT_DAC1 digitalWriteFast(DAC_SYNC1, LOW)
#define DEASSERT_DAC1 digitalWriteFast(DAC_SYNC1, HIGH)

#define ASSERT_DAC2 digitalWriteFast(DAC_SYNC2, LOW)
#define DEASSERT_DAC2 digitalWriteFast(DAC_SYNC2, HIGH)

extern uint16_t transfer16(uint16_t);
extern uint32_t set_arm_clock_cpp(uint32_t); 

#define ADC_TRANSFER_NOP ASSERT_ADC;transfer16(ADC_NOP);DEASSERT_ADC


/**
 * @brief read ADC register
 * 
 * @param c : should be one of ADC_XXX
 * @return uint16_t : the register value
 */
uint16_t adc_read_register(uint16_t c);

/**
 * @brief Choose channel
 * @note this will also turn off sequencer mode 
 */
void adc_set_ch0();
void adc_set_ch1();

/**
 * @brief Turn on/off sequencer mode 
 * @note When turned off, the channel will be what is stored in CH bit 
 * see Table 20 for details
 */
void adc_set_seq();
void adc_unset_seq();

/**
 * @brief Reset ADC 
 */
void adc_reset();

/**
 * @brief Calibrate DAC channel ch with offset and fgain 
 * 
 * @note see section "OFFSET AND GAIN ADJUSTMENT WORKED EXAMPLE" of the datasheet 
 * @note this function should be called *after* prepare_fast_spi_transfer24
 */
void calibrate_dac(uint8_t ch, uint8_t offset, uint8_t fgain); 

#endif