/**
 * @file write.hpp
 * @brief Interface for writing to DAC
 * @date 2023-03-25
 *
 */
#ifndef WRITE_HPP
#define WRITE_HPP

#include "init_chips.hpp"

/**
 * @brief Write num to channel ch
 * 
 * @note ch has to be from {0, 1, 2, 3, 4, 5, 6, 7}
 * 
 */
void write(uint8_t const ch, uint16_t const num);

/**
 * @brief Update two channels simultaneously.
 * @note ch1 has to be from {0, 1, 2, 3} and ch2 from {4, 5, 6, 7}
 */
void write_both(uint8_t const ch1, uint16_t const num1, uint8_t const ch2, uint16_t const num2);

/**
 * @brief Low-level fnction for DAC update
 *
 * @param data 48-bit data to transmit: odd bits for DAC2; even bits for DAC1.
 */
void transfer_dac24(uint64_t const data);
#endif