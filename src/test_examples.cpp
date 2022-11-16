#include "read.hpp"
#include "write.hpp"
#include "bit_mangler.h"

void test_dac() {
    for(int i = 0; i < 64; ++i) {
        write(4, i << 6);
    }
}

void follower_no_dma() {
    static auto spi_regs = &IMXRT_LPSPI4_S; 

    spi_regs -> TCR |= LPSPI_TCR_TXMSK; 
    while(spi_regs -> RSR  & LPSPI_RSR_RXEMPTY);
    auto ch0 = spi_regs -> RDR;
    write(4, decode_32_16(ch0 >> 1));
    write(6, decode_32_16(ch0));

    spi_regs -> TCR |= LPSPI_TCR_TXMSK; 
    while(spi_regs -> RSR  & LPSPI_RSR_RXEMPTY);
    auto ch1 = spi_regs -> RDR;
    write(5, decode_32_16(ch1 >> 1));
    write(7, decode_32_16(ch1));
}

void follow_single_buffer() {
    extern volatile uint32_t ch0, ch1;
    auto const ch0_local = ch0, ch1_local = ch1; 
    write(4, decode_32_16(ch0_local >> 1));
    write(5, decode_32_16(ch1_local >> 1));
    write(6, decode_32_16(ch0_local));
    write(7, decode_32_16(ch1_local));    
}
