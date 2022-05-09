#ifndef SPISLAVE_H
#define SPISLAVE_H

#include <DMAChannel.h>
#include <SPI.h>
#include "init_chips.hpp"

/**
 * @brief Initialize SPI slave with DMA 
 * 
 * @note preferably called before initSPIMaster
 */
void initSPISlave(uint8_t);

#endif 