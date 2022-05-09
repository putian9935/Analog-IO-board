// collection of unused code
#if 0
int cycle(uint16_t num) {
    IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;
    IMXRT_LPSPI_t* spim_regs = &IMXRT_LPSPI4_S;
    spi_regs->TDR = ((((uint32_t)((2 & 3) | DAC_DATA_REG)) << 16)+ (uint32_t)num); 
    spim_regs ->TDR = 0; 
    while ((spim_regs->RSR & LPSPI_RSR_RXEMPTY));
    int ret = spim_regs -> RDR;
    while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY));
    spi_regs -> RDR;
    return ret;
}

  static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;
  static IMXRT_LPSPI_t* spim_regs = &IMXRT_LPSPI4_S;

void follower() {
  
    // delayNanoseconds(200);
    // ASSERT_ADC;
    // delayNanoseconds(200);    
    ASSERT_ADC;
    delayNanoseconds(200);
    DEASSERT_ADC;
    ASSERT_ADC;
    spim_regs ->TDR = 0; 
    ASSERT_DAC1;
    while ((spim_regs->RSR & LPSPI_RSR_RXEMPTY));
    spi_regs->TDR = ((((uint32_t)((2 & 3) | DAC_DATA_REG)) << 16)+ (uint32_t)(spim_regs -> RDR >> 2)); 
    
    DEASSERT_ADC; 
    
    // delayNanoseconds(50);
    while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY));
    spi_regs -> RDR;
    
    DEASSERT_DAC1;
    // return ret;
}

uint16_t follower2(uint16_t data) {
    ASSERT_DAC1;
    spi_regs->TDR = ((((uint32_t)((2 & 3) | DAC_DATA_REG)) << 16)+ (uint32_t)(data)); 
    ASSERT_ADC;
    spim_regs ->TDR = 0; 
    while ((spim_regs->RSR & LPSPI_RSR_RXEMPTY));
    spim_regs -> RDR;
    DEASSERT_ADC;
    ASSERT_ADC;
    spim_regs ->TDR = 0; 
    while ((spim_regs->RSR & LPSPI_RSR_RXEMPTY));
    uint16_t ret  = spim_regs -> RDR;
    DEASSERT_ADC;
    while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY));
    spi_regs -> RDR;
    DEASSERT_DAC1;
    return ret;
}



void follower3() {
    ASSERT_DAC1;
    spi_regs->TDR = ((((uint32_t)((2 & 3) | DAC_DATA_REG)) << 16)+ (uint32_t)(bin0 >> 2)); 
    while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY));
    spi_regs -> RDR;
    DEASSERT_DAC1;
}


uint16_t last_read; 
#endif