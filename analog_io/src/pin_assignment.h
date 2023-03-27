/**
 * @file pin_assignment.h
 * @brief Pin assignment.
 * @date 2022-11-28
 *
 * Give names to pin numbers.
 */

#ifndef PIN_ASSIGNMENT_H
#define PIN_ASSIGNMENT_H

/**
 * @name ADC SPI communication pins 
 */
/** @{ */
#define CS 10
#define MOSI 11
#define MISO 12
#define SCK 13
#define CS2 36 //!< this pin is used as master out during configuration of ADC. 
/** @} */

/**
 * @name DAC SPI communication pins
 */
/** @{ */
#define CS1 0
#define MISO1 1
#define MOSI1 26
#define SCK1 27
/** @} */

/**
 * @name DAC general communication pins 
 */
/** @{ */
#define LDAC1 A10
#define DAC_CLR1 A11

#define LDAC2 29
#define DAC_CLR2 30
/** @} */

/**
 * @name Digital IO pins
 */
/** @{ */
#define DIO1 37
#define DIO2 35
#define DIO3 34
#define DIO4 33

#define DIO5 41
#define DIO6 40
#define DIO7 39
#define DIO8 38
/** @} */
#endif