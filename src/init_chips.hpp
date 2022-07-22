#ifndef INIT_PINS_HPP
#define INIT_PINS_HPP



#include <Arduino.h>
#include <SPI.h>

#include "utilities.hpp"
#include "SPISlave.h"

/*** 
 * ADC input channel selection 
 * define at *most* one of the following
 */
// #define ADC_CH0_ON 
// #define ADC_CH1_ON
#define ADC_SEQ_ON

/***
 * Count ADC sampling rate with interrupt 
 */
// #define COUNT_SAMPLE_RATE
#include "SPIMaster.h"

/***
 * Clock frequency
 * Turning on overclock will increase phase margin to 210khz 
 */
#define OVERCLOCK_ON 

#define MAX_DAC_FCLK 30000000
#define MAX_DAC_FCLK_PRAC 90000000
#define MAX_ADC_FCLK 80000000
#define MAX_ADC_FCLK_PRAC 90000000

#define DAC_FCLK MAX_DAC_FCLK 
#define ADC_FCLK MAX_ADC_FCLK

#ifdef OVERCLOCK_ON
#undef DAC_FCLK
#undef ADC_FCLK
#define DAC_FCLK MAX_DAC_FCLK_PRAC
#define ADC_FCLK MAX_ADC_FCLK_PRAC
#endif 

/*****
 * Pin configuration
 */
#define LDAC1 A4
#define DAC_CLR1 A5
#define DAC_SYNC1 A6

#define LDAC2 A7
#define DAC_CLR2 A8
#define DAC_SYNC2 A9

#define RST_DAC 2

#define CS 10

/***
 * Useful registers 
 */

#define ADC_NOP ((uint16_t) 0)
#define ADC_READ ((uint16_t)(0 << 15))
#define ADC_WRITE ((uint16_t)(1 << 15))

#define ADC_CFG1 ((uint16_t)(1 << 12))
#define ADC_CFG1_CH ((uint16_t)(1 << 11))
#define ADC_CFG1_SEQ ((uint16_t)(1 << 10))

#define ADC_CFG2 ((uint16_t)(2 << 12))
#define ADC_CFG2_SDO ((uint16_t)(1 << 8))
#define ADC_CFG2_SRST ((uint16_t)0x3C)
#define ADC_CFG2_HRST ((uint16_t)0xFF)

#define DAC_FUNC_REG ((uint8_t)(0 << 3))
#define DAC_DATA_REG ((uint8_t)(2 << 3))
#define DAC_CGAIN_REG ((uint8_t)(3 << 3))
#define DAC_FGAIN_REG ((uint8_t)(4 << 3))
#define DAC_OFFSET_REG ((uint8_t)(5 << 3))

#define DAC_CH_A ((uint8_t)0)
#define DAC_CH_B ((uint8_t)1)
#define DAC_CH_C ((uint8_t)2)
#define DAC_CH_D ((uint8_t)3)
#define DAC_CH_ALL ((uint8_t)4)

/**
 * @brief initialize all chips
 */
void init_chips();

void set_fastio_pin(uint8_t);
#endif