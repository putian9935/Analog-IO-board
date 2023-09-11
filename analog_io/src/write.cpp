#include "write.hpp"

#define DAC8564_CHANNEL(x) ((x) << 17)
#define DAC8564_IMMED_WRITE (1 << 20)

static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;

void async_write(uint8_t const ch, uint16_t const num) {
    while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
        ;
    spi_regs->RDR;
    spi_regs->TDR = (DAC8564_IMMED_WRITE | DAC8564_CHANNEL(ch) | num);
}