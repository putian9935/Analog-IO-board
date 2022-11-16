#ifndef UTILITIES_H
#define UTILITIES_H

<<<<<<< HEAD
#include "init_chips.hpp"
#include "bit_mangler.h"
#include "write.hpp"

#include <SPI.h>
=======
>>>>>>> new-board
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


#define FAST_IO IOMUXC_PAD_DSE(4) | IOMUXC_PAD_SPEED(3) | IOMUXC_PAD_SRE

/**
 * @brief Reset ADC 
 * 
 * @note this function must be called before end_fourway
 */
<<<<<<< HEAD
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


#define ADC_CFG1_OSR_MASK (7 << 6)
#define ADC_CFG1_OSR(x) ((1 << (x)) << 6)

void adc_set_oversampling(uint8_t);

=======
>>>>>>> new-board
void adc_reset();

void set_fastio_pin(uint8_t pin_num); 

/**
 * @brief Calibrate DAC channel ch with offset and fgain 
 * 
 * @note see section "OFFSET AND GAIN ADJUSTMENT WORKED EXAMPLE" of the datasheet 
 * @note this function should be called *after* init_DAC_spi
 */
void calibrate_dac(uint8_t ch, uint8_t offset, uint8_t fgain); 

#endif