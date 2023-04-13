#include "init_adc.hpp"
#include <SPI.h>
#include <stdint.h>
#include "bit_mangler.h"
#include "init_chips.hpp"
#include "pin_assignment.h"

#define ALT2 2
#define ALT5 5

static void start_fourway() {
    static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI4_S;

    uint32_t tcr  = spi_regs->TCR;
    spi_regs->TCR = (tcr & 0xfffff000) | LPSPI_TCR_FRAMESZ(63) | LPSPI_TCR_RXMSK | LPSPI_TCR_WIDTH(2);

    // set datawidth to 4
    spi_regs->CFGR1 |= (LPSPI_CFGR1_PINCFG(2) | LPSPI_CFGR1_PCSCFG);

    // turn on cs2 36
    pinMode(CS2, OUTPUT);
    IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_02 = ALT2;
}

static void end_fourway() {
    // turn off cs2 36
    IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_02 = ALT5;
    digitalWrite(CS2, LOW);

    // set datawidth to 2
    static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI4_S;

    spi_regs->CFGR1 &= ~LPSPI_CFGR1_PCSCFG;
    spi_regs->CFGR1 |= LPSPI_CFGR1_OUTCFG | LPSPI_CFGR1_SAMPLE | LPSPI_CFGR1_MASTER;

    spi_regs->TCR = ((spi_regs->TCR & 0xfff00000) | LPSPI_TCR_FRAMESZ(31) | LPSPI_TCR_WIDTH(1));

#ifdef OVERCLOCK_ON
    spi_regs->CCR = (spi_regs->CCR & 0x000000ff) | LPSPI_CCR_DBT(30) | LPSPI_CCR_PCSSCK(100) | LPSPI_CCR_SCKPCS(1);
#else
    spi_regs->CCR = (spi_regs->CCR & 0x000000ff) | LPSPI_CCR_DBT(10) | LPSPI_CCR_PCSSCK(1) | LPSPI_CCR_SCKPCS(1);
#endif
}

static void fourway_write(uint16_t word) {
    static uint8_t const CS2_offset = 2;
    static IMXRT_LPSPI_t* spi_regs  = &IMXRT_LPSPI4_S;

    uint32_t low  = encode_8_32(word & 0xFF) << CS2_offset,
             high = encode_8_32((word & 0xFF00) >> 8) << CS2_offset;
    while ((spi_regs->FSR & 0xff) > 14)
        ;
    spi_regs->TDR = high;
    spi_regs->TDR = low;
}

void init_ADC() {
    SPI.begin();
    SPI.beginTransaction(SPISettings(ADC_FCLK, MSBFIRST, SPI_MODE2));
    SPI.setCS(CS);

    set_fastio_pin(CS);
    set_fastio_pin(MOSI);
    set_fastio_pin(MISO);
    set_fastio_pin(SCK);

    start_fourway();

    // Hardware reset
    fourway_write((uint16_t)(ADC_WRITE | ADC_CFG2 | ADC_CFG2_HRST));
    delay(10);
    fourway_write(ADC_NOP);
    fourway_write(ADC_NOP);

#ifdef ADC_FOUR_CHANNEL
    // Turn on sequencer mode; rolling average; average size 2**3=8
    fourway_write((uint16_t)(ADC_WRITE | ADC_CFG1 | ADC_CFG1_SEQ | ADC_CFG1_ROLLING | ADC_CFG1_OSR(1)));
#else
    // Rolling average; average size 2**3=8
    fourway_write((uint16_t)(ADC_WRITE | ADC_CFG1 | ADC_CFG1_ROLLING | ADC_CFG1_OSR(1)));
#endif
    delay(1);
    fourway_write(ADC_NOP);
    fourway_write(ADC_NOP);
    end_fourway();
    delay(10);
}
