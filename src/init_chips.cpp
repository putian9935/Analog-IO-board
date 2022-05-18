#include "init_chips.hpp"

#define FAST_IO IOMUXC_PAD_DSE(4) | IOMUXC_PAD_SPEED(3) | IOMUXC_PAD_SRE

void set_fastio_pin(uint8_t pin_num) {
	*(portControlRegister(pin_num)) = FAST_IO;
}

static void prepare_fast_spi_transfer24(){
    IOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_02 = (IOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_02 & ~0x7) | 5; 
    pinMode(1, INPUT); // old MISO
    pinMode(39, OUTPUT); // new MISO as output
    SPI1.setCS(0);
    SPI1.setMISO(39);
    set_fastio_pin(0);
    set_fastio_pin(39);
    IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;

    // running at 36MHz
    uint16_t div = 720000000 / MAX_DAC_FCLK_PRAC;
    spi_regs -> CCR = LPSPI_CCR_SCKDIV(div-2) | LPSPI_CCR_SCKPCS(2);
    
    uint32_t tcr = spi_regs -> TCR;
    spi_regs -> TCR = (tcr & 0xfffff000) | LPSPI_TCR_FRAMESZ(47) | LPSPI_TCR_RXMSK | LPSPI_TCR_WIDTH(1);
    spi_regs->CFGR1 |= LPSPI_CFGR1_PINCFG(1);
}

static void init_DAC1()
{
    SPI1.begin();
    SPI1.beginTransaction(SPISettings(MAX_DAC_FCLK, MSBFIRST, SPI_MODE1));
    pinMode(LDAC1, OUTPUT);
    pinMode(DAC_CLR1, OUTPUT);
    digitalWrite(DAC_CLR1, HIGH); 

    set_fastio_pin(26);  // MOSI1
    set_fastio_pin(17);  // SCK1
}

static void init_DAC2()
{
    pinMode(LDAC2, OUTPUT);
    pinMode(DAC_CLR2, OUTPUT);
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


