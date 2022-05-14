#include "write.hpp"

static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;

/*
 * @brief Fast spi transfer16 for DAC, cf. SPI.h implementation
 *
 * @param data
 * @return uint16_t
 */
static void transfer_dac24(uint64_t data)
{
    Serial.println(IOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_02&7);
    Serial.println(IOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_13&7);
    while(spi_regs -> FSR & 0xff > 15);
    spi_regs->TDR = (data & 0xFFFFFFFF0000) >> 16;
    spi_regs->TDR = (data & 0xFFFF);
}

void write(uint8_t ch, uint16_t num)
{
    static uint64_t dac1_num_mangled, dac2_num_mangled;

    uint64_t new_dac_num = insert_zeros(((((uint32_t)((ch & 3) | DAC_DATA_REG)) << 16) + num));
    if (ch < 4)
        dac1_num_mangled = new_dac_num;
    else
        dac2_num_mangled = new_dac_num << 1; 
        
    transfer_dac24((dac1_num_mangled | dac2_num_mangled));
}