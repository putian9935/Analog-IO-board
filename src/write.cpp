#include "write.hpp"

static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;

/*
 * @brief Fast spi transfer16 for DAC, cf. SPI.h implementation
 *
 * @param data
 * @return uint16_t
 */
static void transfer_dac24(uint32_t data)
{
    spi_regs->TDR = data;
    while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY));
    spi_regs -> RDR;
}

FASTRUN void write(uint8_t ch, uint16_t num)
{
    if (ch < 4)
        ASSERT_DAC1;
    else
        ASSERT_DAC2;

    transfer_dac24(((((uint32_t)((ch & 3) | DAC_DATA_REG)) << 16) + num));
    
    if (ch < 4)
        DEASSERT_DAC1;
    else
        DEASSERT_DAC2;
}