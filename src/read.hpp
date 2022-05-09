
#ifndef READ_HPP
#define READ_HPP 

#include <SPI.h> 
#include "init_chips.hpp"
#include "utilities.hpp"


extern volatile uint32_t ain0, ain1, bin0, bin1;

#if 0
typedef struct {
    uint16_t a; 
    uint16_t b;
} pair; 

typedef struct {
    uint16_t a0; 
    uint16_t b0;
    uint16_t a1; 
    uint16_t b1;
} quad; 

/**
 * 
 * @brief standard read; two cycles needed 
 */
uint16_t read();
pair read_pair();
quad read_quad();

/**
 * 
 * @brief fast read; one cycles needed 
 */
uint16_t read_high_throughput();
pair read_pair_high_throughput();
quad read_quad_high_throughput();

extern void prepare_slave_read();
extern uint16_t slave_read();
#endif 

#endif

