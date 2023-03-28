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
extern decltype(write_both)& analogio_write_both;

/**
 * @brief Alias of \link write.hpp write\endlink.
 *
 * @copydoc write
 */
extern decltype(write)& analogio_write;
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
extern decltype(digitalReadFast)& digitalio_read;
/**
 * @brief Alias of \c digitalWriteFast of Teensy.
 *
 * The usage is the same as Arduino's <a href="https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/">digitalWrite</a>.
 */
extern decltype(digitalWriteFast)& digitalio_write;
/**
 * @brief Alias of \c pinMode of Teensy.
 *
 * The usage is the same as Arduino's <a href="https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/">pinMode</a>.
 */
extern decltype(pinMode)& digitalio_mode;
// @}

#endif