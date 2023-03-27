/**
 * @file analog_io.h
 * @author Li Putian
 * @brief Master header for analog IO library
 * @version 0.1
 * @date 2023-03-27
 *
 */

#ifndef ANALOG_IO_H
#define ANALOG_IO_H

#ifndef ADC_TWO_CHANNEL
#    define ADC_FOUR_CHANNEL
#endif

#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"

/**
 * \name Analog Read
 * @{
 */

/**
 * \name ADC single read
 * @{
 */
#define AIN0 0
#define BIN0 1
#define AIN1 2
#define BIN1 3

/**
 * @brief Read from channel ch 
 * 
 * @param ch ADC channel, must be either of AIN0, BIN0, AIN1, BIN1 
 * @return uint16_t the ADC number of the requested channel 
 * 
 */
uint16_t analogio_read(uint8_t const ch);
// @}

/**
 * \name ADC pair read
 * @{
 */
#define CH0 0
#define CH1 1

/**
 * @brief Read from a pair of channel ch 
 * 
 * @param ch ADC channel pair, must be either of CH0 (for AIN0, BIN0) or CH1 (for AIN1, BIN1) 
 * @return std::pair<uint16_t, uint16_t>  the ADC number of the requested channels, with first element 
 * being A and second element being B. 
 *   
 */
std::pair<uint16_t, uint16_t> analogio_read_both(uint8_t const ch);
// @}

// @}

/**
 * \name Analog Write
 * @{
 */

/**
 * @brief Alias of \link write.hpp write_both\endlink.
 *
 * @copydoc write_both
 */
auto &analogio_write_both = write_both;

/**
 * @brief Alias of \link write.hpp write\endlink.
 *
 * @copydoc write
 */
auto &analogio_write = write;
// @}

/**
 * @name Digital Read
 *
 */
// @{
/**
 * @brief Alias of \c digitalReadFast of Teensy.
 *
 * The usage is the same as Arduino's <a href="https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/">digitalRead</a>. 
 */
auto &digitalio_read = digitalReadFast;
/**
 * @brief Alias of \c digitalWriteFast of Teensy.
 *
 * The usage is the same as Arduino's <a href="https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/">digitalWrite</a>. 
 */
auto &digitalio_write = digitalWriteFast;
/**
 * @brief Alias of \c pinMode of Teensy.
 *
 * The usage is the same as Arduino's <a href="https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/">pinMode</a>. 
 */
auto &digitalio_mode = pinMode;
// @}

#endif