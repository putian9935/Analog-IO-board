#ifndef SHOW_CCM_REGISTERS_HPP
#define SHOW_CCM_REGISTERS_HPP

#include <Arduino.h> 
#include "show_byte.hpp"

void show_ipg_clk(); 
void show_lpspi_clk();

void show_gpio_reg(uint32_t);
#endif 