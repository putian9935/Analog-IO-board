#include "write.hpp"

static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;

/*
 * @brief Fast spi transfer16 for DAC, cf. SPI.h implementation
 *
 * @param data
 * @return uint16_t
 */
void transfer_dac24(uint64_t data)
{
    while ((spi_regs->FSR & 0xff) > 15)
        ;
    spi_regs->TDR = data >> 16;
    spi_regs->TDR = (data & 0xFFFF);
}

void write(uint8_t ch, uint16_t num)
{
    static uint64_t dac1_num_mangled, dac2_num_mangled;

    uint64_t new_dac_num = insert_zeros(((((uint32_t)((ch & 3) | DAC_DATA_REG)) << 16) | num));

    if (ch < 4)
        dac1_num_mangled = new_dac_num << 1;
    else
        dac2_num_mangled = new_dac_num;

    transfer_dac24((dac1_num_mangled | dac2_num_mangled));
}

void write_both(uint8_t ch1, uint16_t num1, uint8_t ch2, uint16_t num2)
{
    uint64_t dac1_num_mangled, dac2_num_mangled;

    dac1_num_mangled = insert_zeros(((((uint32_t)((ch1 & 3) | DAC_DATA_REG)) << 16) | num1)) << 1;
    dac2_num_mangled = insert_zeros(((((uint32_t)((ch2 & 3) | DAC_DATA_REG)) << 16) | num2));

    transfer_dac24((dac1_num_mangled | dac2_num_mangled));
}
