#ifndef WRITE_HPP
#define WRITE_HPP

#include "init_chips.hpp"

/**
 * @brief Write num to channel ch
 */
void write(uint8_t const ch, uint16_t const num);
// void write(uint8_t const ch, double const num); 

/**
 * @brief Update two channels simultaneously. 
 * @note ch1 has to be from {0, 1, 2, 3} ad ch2 from {4, 5, 6, 7}
 */
void write_both(uint8_t ch1, uint16_t num1, uint8_t ch2, uint16_t num2);

/**
 * @brief Low-level fnction for DAC update 
 * 
 * @param data 48-bit data to transmit: odd bits are for DAC2 while even bits for DAC1. 
 */
void transfer_dac24(uint64_t data);
#endif 