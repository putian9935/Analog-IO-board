#include "init_chips.hpp"
#include "utilities.hpp"
#include "pin_assignment.h"

static void prepare_fast_spi_transfer24()
{
    SPI1.begin();
    SPI1.beginTransaction(SPISettings(MAX_DAC_FCLK, MSBFIRST, SPI_MODE1));
    SPI1.setCS(CS1);
    SPI1.setMISO(MISO1);

    set_fastio_pin(CS1);
    set_fastio_pin(MISO1);
    set_fastio_pin(MOSI1);
    set_fastio_pin(SCK1); 

    IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;
    uint16_t div            = 720000000 / MAX_DAC_FCLK_PRAC;

    spi_regs->CCR = LPSPI_CCR_SCKDIV(div - 2) | LPSPI_CCR_DBT(55) | LPSPI_CCR_PCSSCK(35) | LPSPI_CCR_SCKPCS(0);

    // uint32_t tcr  = ;
    spi_regs->TCR = (spi_regs->TCR & 0xfffff000)      //
                    | LPSPI_TCR_FRAMESZ(47) //
                    | LPSPI_TCR_RXMSK       //
                    | LPSPI_TCR_WIDTH(1);

    spi_regs->CFGR1 |= LPSPI_CFGR1_PINCFG(1);
}

static void init_DAC1()
{
    pinMode(LDAC1, OUTPUT);
    digitalWrite(LDAC1, LOW);
    pinMode(DAC_CLR1, OUTPUT);
    digitalWrite(DAC_CLR1, HIGH);
}

static void init_DAC2()
{
    pinMode(LDAC2, OUTPUT);
    digitalWrite(LDAC2, LOW);
    pinMode(DAC_CLR2, OUTPUT);
    digitalWrite(DAC_CLR2, HIGH);
}

static void init_ADC()
{
}

void init_chips()
{
    // This will set a higher IPG root clock
    CCM_CBCDR = (CCM_CBCDR & ~CCM_CBCDR_IPG_PODF_MASK) | CCM_CBCDR_IPG_PODF(1);
    
    CCM_CBCMR = (CCM_CBCMR & ~(CCM_CBCMR_LPSPI_PODF_MASK | CCM_CBCMR_LPSPI_CLK_SEL_MASK)) |
                CCM_CBCMR_LPSPI_PODF(0) | CCM_CBCMR_LPSPI_CLK_SEL(1);

    // increase CPU clock speed to 800MHz
    set_arm_clock_cpp(800000000);

    init_DAC1();
    init_DAC2();

    prepare_fast_spi_transfer24();

    delay(20); // wait for chips start-up

    // init_ADC();

    // calibration of dac starts from here

    delay(200);
}
