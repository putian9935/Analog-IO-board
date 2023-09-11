/**
 * @file init_chips.hpp
 * @brief Init board. 
 * @date 2022-11-28
 * 
 */
#ifndef INIT_CHIPS_HPP
#define INIT_CHIPS_HPP

#include <Arduino.h>


#ifndef ADC_TWO_CHANNEL
#    define ADC_FOUR_CHANNEL
#endif

#define ADC_SEQ_ON

/***
 * Clock frequency
 * Turning on overclock will increase phase margin to 210khz 
 */
#define OVERCLOCK_ON 

#define MAX_DAC_FCLK 50000000
#define MAX_DAC_FCLK_PRAC 66000000
#define MAX_ADC_FCLK 50000000
#define MAX_ADC_FCLK_PRAC 66000000

#define DAC_FCLK MAX_DAC_FCLK 
#define ADC_FCLK MAX_ADC_FCLK

#ifdef OVERCLOCK_ON
#undef DAC_FCLK
#undef ADC_FCLK
#define DAC_FCLK MAX_DAC_FCLK_PRAC
#define ADC_FCLK MAX_ADC_FCLK_PRAC
#endif 


/***
 * Useful registers 
 */

#define ADC_NOP ((uint16_t) 0)
#define ADC_READ ((uint16_t)(0 << 15))
#define ADC_WRITE ((uint16_t)(1 << 15))

#define ADC_CFG1 ((uint16_t)(1 << 12))
#define ADC_CFG1_SEQ ((uint16_t)(1 << 10))
#define ADC_CFG1_ROLLING ((uint16_t)(1 << 9))
#define ADC_CFG1_OSR(x) ((uint16_t)((x) << 6))

#define ADC_CFG2 ((uint16_t)(2 << 12))
#define ADC_CFG2_SDO ((uint16_t)(1 << 8))
#define ADC_CFG2_SRST ((uint16_t)0x3C)
#define ADC_CFG2_HRST ((uint16_t)0xFF)

/**
 * @brief initialize all chips
 */
void init_chips();

/**
 * @brief Change pin driver settings for high slew rate and better impedance matching 
 * this will generally help to provide a better signal integrity on the board 
 */
void set_fastio_pin(uint8_t pin);
#endif