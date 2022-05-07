#include "init_chips.hpp"


void prepare_fast_spi_transfer24(){
    IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;
    spi_regs -> CCR = (spi_regs -> CCR & 0xff);
    uint32_t tcr = spi_regs -> TCR;
    spi_regs -> TCR = (tcr & 0xfffff000) | LPSPI_TCR_FRAMESZ(23);
}

void init_chips()
{
    init_DAC1();
    init_DAC2();
    init_ADC();
    

    pinMode(RST_DAC, OUTPUT);
    digitalWrite(RST_DAC, HIGH);

    digitalWrite(LDAC1, LOW);
    digitalWrite(LDAC2, LOW);
    prepare_fast_spi_transfer24();

    adc_reset(); 
}



void init_DAC1()
{
    SPI1.begin();
    SPI1.beginTransaction(SPISettings(MAX_DAC_FCLK, MSBFIRST, SPI_MODE1));
    pinMode(LDAC1, OUTPUT);
    pinMode(DAC_CLR1, OUTPUT);
    pinMode(DAC_SYNC1, OUTPUT);

    digitalWrite(DAC_SYNC1, HIGH);
    digitalWrite(DAC_CLR1, HIGH); 

}

void init_DAC2()
{
    pinMode(LDAC2, OUTPUT);
    pinMode(DAC_CLR2, OUTPUT);
    pinMode(DAC_SYNC2, OUTPUT);

    digitalWrite(DAC_SYNC2, HIGH);
    digitalWrite(DAC_CLR2, HIGH); 
}


void prepare_fast_spi_transfer16(){ 
    IMXRT_LPSPI_t* spim_regs    = &IMXRT_LPSPI4_S;
    spim_regs -> CCR = (spim_regs -> CCR & 0xff);
    uint32_t tcr = spim_regs -> TCR;
    spim_regs -> TCR = (tcr & 0xfffff000) | LPSPI_TCR_FRAMESZ(15);
}

void init_ADC()
{
    SPI.begin();
    SPI.beginTransaction(SPISettings(MAX_ADC_FCLK_PRAC, MSBFIRST, SPI_MODE2));
    prepare_fast_spi_transfer16();
    pinMode(CS, OUTPUT); 
    digitalWrite(CS, HIGH);
    
    initSPISlave(SPI_MODE2);
}