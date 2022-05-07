#ifndef WRITE_HPP
#define WRITE_HPP

#include "init_chips.hpp"
#include <SPI.h>
#include <Arduino.h>

/**
 * @brief Write num to channel
 * 
 * @param ch 
 * @param num 
 */
void write(uint8_t ch, uint16_t num);

#endif 