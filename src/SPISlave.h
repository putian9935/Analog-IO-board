#ifndef SPISLAVE_H
#define SPISLAVE_H

#include <DMAChannel.h>
#include <SPI.h>


void initSPISlave(uint8_t);
void prepare_slave_read();
uint16_t slave_read(); 

#endif 