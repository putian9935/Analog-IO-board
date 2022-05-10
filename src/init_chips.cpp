#include "init_chips.hpp"


static void prepare_fast_spi_transfer24(){
    IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;
    spi_regs -> CCR = (spi_regs -> CCR & 0xff);
    uint32_t tcr = spi_regs -> TCR;
    spi_regs -> TCR = (tcr & 0xfffff000) | LPSPI_TCR_FRAMESZ(23);
}

static void init_DAC1()
{
    SPI1.begin();
    SPI1.beginTransaction(SPISettings(MAX_DAC_FCLK, MSBFIRST, SPI_MODE1));
    pinMode(LDAC1, OUTPUT);
    pinMode(DAC_CLR1, OUTPUT);
    pinMode(DAC_SYNC1, OUTPUT);

    digitalWrite(DAC_SYNC1, HIGH);
    digitalWrite(DAC_CLR1, HIGH); 

    prepare_fast_spi_transfer24();
}

static void init_DAC2()
{
    pinMode(LDAC2, OUTPUT);
    pinMode(DAC_CLR2, OUTPUT);
    pinMode(DAC_SYNC2, OUTPUT);

    digitalWrite(DAC_SYNC2, HIGH);
    digitalWrite(DAC_CLR2, HIGH); 
}


static void init_ADC()
{
    initSPISlave(SPI_MODE2);
    initSPIMaster(SPI_MODE2);
}

void init_chips()
{
    init_DAC1();
    init_DAC2();
    pinMode(RST_DAC, OUTPUT);
    digitalWrite(RST_DAC, HIGH);

    digitalWrite(LDAC1, LOW);
    digitalWrite(LDAC2, LOW);
    init_ADC();
    
    prepare_fast_spi_transfer24();
}


