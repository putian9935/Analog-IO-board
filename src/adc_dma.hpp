#ifndef ADC_DMA_HPP
#define ADC_DMA_HPP

#include <stdint.h>

void initSPIMasterDMA();
extern volatile uint32_t ch0, ch1;

#endif 