#include "write.hpp"

#include "bit_mangler.h"

static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;

void transfer_dac24(uint64_t const data)
{
    while ((spi_regs->FSR & 0xff) > 14)
        ;
    spi_regs->TDR = data >> 16;
    spi_regs->TDR = (data & 0xFFFF);
}

void write(uint8_t const ch, uint16_t const num)
{
    static uint64_t dac1_num_mangled, dac2_num_mangled;

    uint64_t new_dac_num = insert_zeros(((((uint32_t)((ch & 3) | DAC_DATA_REG)) << 16) | num));

    if (ch < 4)
        dac1_num_mangled = new_dac_num << 1;
    else
        dac2_num_mangled = new_dac_num;

    transfer_dac24((dac1_num_mangled | dac2_num_mangled));
}

void write_both(uint8_t const ch1, uint16_t const num1, uint8_t const ch2, uint16_t const num2)
{
    uint64_t dac1_num_mangled, dac2_num_mangled;

    dac1_num_mangled = insert_zeros(((((uint32_t)((ch1 & 3) | DAC_DATA_REG)) << 16) | num1)) << 1;
    dac2_num_mangled = insert_zeros(((((uint32_t)((ch2 & 3) | DAC_DATA_REG)) << 16) | num2));

    transfer_dac24((dac1_num_mangled | dac2_num_mangled));
}
