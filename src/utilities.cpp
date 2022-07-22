#include "utilities.hpp"

void adc_reset()
{
    ASSERT_ADC;
    SPI.transfer16((uint16_t)(ADC_WRITE | ADC_CFG2 | ADC_CFG2_HRST));
    DEASSERT_ADC;

    ADC_TRANSFER_NOP; // discard the result of first two transfers
    ADC_TRANSFER_NOP;

    delay(1);
}

uint16_t adc_read_register(uint16_t c)
{
    ASSERT_ADC;
    SPI.transfer16((uint16_t)c);
    DEASSERT_ADC;
    ASSERT_ADC;
    uint16_t ret = SPI.transfer16(ADC_NOP);
    DEASSERT_ADC;
    return ret;
}

void adc_set_ch0()
{
    uint16_t old = adc_read_register(ADC_CFG1);

    ASSERT_ADC;
    SPI.transfer16((uint16_t)(ADC_WRITE | ADC_CFG1 | old & (~ADC_CFG1_SEQ | ~ADC_CFG1_CH)));
    DEASSERT_ADC;

    // discard the result of first two transfers, see Figure 31
    ADC_TRANSFER_NOP;
    ADC_TRANSFER_NOP;
}

void adc_set_ch1()
{
    uint16_t old = adc_read_register(ADC_CFG1);

    ASSERT_ADC;
    SPI.transfer16((uint16_t)((ADC_WRITE | ADC_CFG1 | old | ADC_CFG1_CH) & (~ADC_CFG1_SEQ)));
    DEASSERT_ADC;

    // discard the result of first two transfers, see Figure 31
    ADC_TRANSFER_NOP;
    ADC_TRANSFER_NOP;
}

void adc_set_oversampling(uint8_t osr) {
    uint16_t old = adc_read_register(ADC_CFG1);

    ASSERT_ADC;
    SPI.transfer16((uint16_t) ((ADC_WRITE | ADC_CFG1 | old) & ~ADC_CFG1_OSR_MASK | ADC_CFG1_OSR(3)));
    DEASSERT_ADC; 
    
    // discard the result of first two transfers, see Figure 31
    ADC_TRANSFER_NOP; 
    ADC_TRANSFER_NOP;
}

void adc_set_seq() {
    uint16_t old = adc_read_register(ADC_CFG1);

    ASSERT_ADC;
    SPI.transfer16((uint16_t)(ADC_WRITE | ADC_CFG1 | old | ADC_CFG1_SEQ));
    DEASSERT_ADC;

    // discard the result of first two transfers, see Figure 32
    ADC_TRANSFER_NOP;
    ADC_TRANSFER_NOP;
    ADC_TRANSFER_NOP;
    ADC_TRANSFER_NOP;
}

void adc_unset_seq()
{
    uint16_t old = adc_read_register(ADC_CFG1);

    ASSERT_ADC;
    SPI.transfer16((uint16_t)(ADC_WRITE | ADC_CFG1 | old & (~ADC_CFG1_SEQ)));
    DEASSERT_ADC;

    // discard the result of first two transfers, see Figure 32
    ADC_TRANSFER_NOP;
    ADC_TRANSFER_NOP;
}

void calibrate_DAC1()
{
    uint64_t new_dac_num = insert_zeros(((((uint32_t)((2 & 3) | DAC_OFFSET_REG)) << 16) | (uint8_t)(-11)));
    transfer_dac24(new_dac_num << 1);

    new_dac_num = insert_zeros(((((uint32_t)((2 & 3) | DAC_FGAIN_REG)) << 16) | (uint8_t)(-20)));
    transfer_dac24(new_dac_num << 1);
}
