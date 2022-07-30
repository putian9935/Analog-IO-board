
#ifndef READ_HPP
#define READ_HPP 

#include "init_chips.hpp"

// low-level read variable 
extern volatile uint32_t ain0, ain1, bin0, bin1;

// low-level read wrapped as function  
uint16_t read_ain0();
uint16_t read_ain1();
uint16_t read_bin0();
uint16_t read_bin1();
#endif

