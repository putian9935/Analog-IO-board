#ifndef WRITE_HPP
#define WRITE_HPP

#include "init_chips.hpp"
#include <SPI.h>
#include <Arduino.h>
#include "bit_mangler.h"

/**
 * @brief Write num to channel
 * 
 * @param ch 
 * @param num 
 */
void write(uint8_t ch, uint16_t num);

void write_both(uint8_t ch1, uint16_t num1, uint8_t ch2, uint16_t num2);

#endif 