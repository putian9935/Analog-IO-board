#ifndef INIT_PINS_HPP
#define INIT_PINS_HPP

#include <Arduino.h>
#include <SPI.h>

#include "utilities.hpp"
#include "SPISlave.h"

#define MAX_DAC_FCLK 30000000
#define MAX_ADC_FCLK 80000000
#define MAX_ADC_FCLK_PRAC 60000000

#define LDAC1 A4
#define DAC_CLR1 A5
#define DAC_SYNC1 A6

#define LDAC2 A7
#define DAC_CLR2 A8
#define DAC_SYNC2 A9

#define RST_DAC 2

#define CS 10

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

/**
 * \brief initialize DAC1, IC8
 * \note both DAC1 and DAC2 share the same set of SPI pins
 **/
void init_DAC1();

/**
 * @brief initialize DAC2, IC4
 * \note both DAC1 and DAC2 share the same set of SPI pins
 */
void init_DAC2();

/**
 * @brief initialize ADC
 */
void init_ADC();

void write_master();
#endif