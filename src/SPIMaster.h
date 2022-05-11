#ifndef SPIMASTER_H
#define SPIMASTER_H

#include <DMAChannel.h>
#include <SPI.h>
#include "init_chips.hpp"

#ifdef COUNT_SAMPLE_RATE 
volatile uint32_t cnt_r, cnt_t;
#endif 

/**
 * @brief Initialize ADC SPI master with DMA setup
 * 
 * @param dataMode one of SPI_MODEx(SPI_MODE2 for AD7396)
 * @note preferably called after initSPISlave
 */
void initSPIMaster(uint8_t);

#endif 