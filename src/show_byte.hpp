/**
 * @file show_byte.hpp
 * @brief Helper macro and functions for print pretty-looking binary values 
 * @date 2022-11-28
 * 
 * 
 */
#ifndef SHOW_BYTE_HPP
#define SHOW_BYTE_HPP

#define PRREG(x)            \
    Serial.print(#x " 0x"); \
    Serial.println(x, BIN) // Serial.println(x,BIN)
    
template<typename T>
void printBinary(T value); 

#include "show_byte.cpp"

#endif 