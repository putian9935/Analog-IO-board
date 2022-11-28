/**
 * @file show_registers.hpp
 * @brief Helper functions to show registers of related modules
 * @date 2022-11-28
 * 
 * 
 */
#ifndef SHOW_REGISTERS_HPP
#define SHOW_REGISTERS_HPP

#include <Arduino.h> 

void show_ipg_clk(); 
void show_lpspi_clk();

void show_gpio_reg(uint32_t);
#endif 