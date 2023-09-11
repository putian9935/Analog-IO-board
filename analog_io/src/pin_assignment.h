/**
 * @file pin_assignment.h
 * @brief Pin assignment.
 * @date 2023-9-11
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
#define DAC_LDAC 24
#define DAC_ENABLE 25
/** @} */

/**
 * @name Digital IO pins
 * @todo 
 */
/** @{ */
#define DIO1 41
#define DIO2 40
#define DIO3 39
#define DIO4 38

#define DIO5 37
#define DIO6 36
/** @} */
#endif