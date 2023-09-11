/**
 * @file read.hpp
 * @brief Interface for getting ADC readings
 * @date 2022-11-28
 *
 */
#ifndef READ_HPP
#define READ_HPP

#include <stdint.h>

#include "init_chips.hpp"

/**
 * @name ADC reader functions
 *
 */

/** @{ */

/**
 * @brief Read ain0 from cache
 *
 * @return The 16-bit ADC reading
 *
 */
uint16_t read_ain0();

/**
 * @brief Read bin0 from cache
 *
 * @return The 16-bit ADC reading
 *
 */
uint16_t read_bin0();

#ifdef ADC_FOUR_CHANNEL
/**
 * @brief Read ain1 from cache
 *
 * @return The 16-bit ADC reading
 *
 */
uint16_t read_ain1();
/**
 * @brief Read bin1 from cache
 *
 * @return The 16-bit ADC reading
 *
 */
uint16_t read_bin1();
#else
#warning ADC_FOUR_CHANNEL is not defined!
#endif
/**@}*/
#endif
