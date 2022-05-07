#ifndef SHOW_BYTE_HPP
#define SHOW_BYTE_HPP

#define PRREG(x)            \
    Serial.print(#x " 0x"); \
    Serial.println(x, BIN) // Serial.println(x,BIN)
    
template<typename T>
void printBinary(T value); 

#include "show_byte.cpp"

#endif 