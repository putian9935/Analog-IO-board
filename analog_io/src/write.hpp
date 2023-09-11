/**
 * @file write.hpp
 * @brief Interface for writing to DAC
 * @date 2023-03-25
 *
 */
#ifndef WRITE_HPP
#define WRITE_HPP

#include <SPI.h>

/**
 * @brief Write num to channel ch
 * 
 * @note ch has to be from {0, 1, 2, 3, 4, 5, 6, 7}
 * 
 */
void async_write(uint8_t const ch, uint16_t const num);

#endif