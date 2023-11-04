#include "init_adc.hpp"

#include <SPI.h>
#include <stdint.h>

#include "bit_mangler.h"
#include "init_chips.hpp"
#include "pin_assignment.h"

#define ALT2 2
#define ALT5 5

#include <DMAChannel.h>

static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI4_S;
volatile uint32_t ain0, ain1 ;

DMAChannel rx, tx;
static uint32_t txb;

static void start_fourway() {
    uint32_t tcr = spi_regs->TCR;
    spi_regs->TCR = (tcr & 0xfffff000) | LPSPI_TCR_FRAMESZ(63) |
                    LPSPI_TCR_RXMSK | LPSPI_TCR_WIDTH(2);

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
    spi_regs->CR &= ~LPSPI_CR_MEN;

    // set datawidth to 2

    spi_regs->CFGR1 &= ~LPSPI_CFGR1_PCSCFG;
    spi_regs->CFGR1 |=
        LPSPI_CFGR1_OUTCFG | LPSPI_CFGR1_SAMPLE | LPSPI_CFGR1_MASTER;

    txb = ((spi_regs->TCR & 0xfff00000) | LPSPI_TCR_FRAMESZ(31) |
           LPSPI_TCR_WIDTH(1) | LPSPI_TCR_TXMSK);
    spi_regs->TCR = txb;

#ifdef OVERCLOCK_ON
    spi_regs->CCR = LPSPI_CCR_SCKDIV(0) | LPSPI_CCR_DBT(20) |
                    LPSPI_CCR_PCSSCK(1) | LPSPI_CCR_SCKPCS(0);
#else
    spi_regs->CCR = (spi_regs->CCR & 0x000000ff) | LPSPI_CCR_DBT(10) |
                    LPSPI_CCR_PCSSCK(2) | LPSPI_CCR_SCKPCS(2);
#endif

    spi_regs->DER = LPSPI_DER_RDDE | LPSPI_DER_TDDE;

    rx.begin(true);
    rx.source(spi_regs->RDR);
    rx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI4_RX);
    rx.destination(ain0);
#ifdef ADC_SEQ_ON
    rx.transferCount(2);
    rx.TCD->DOFF = 4;
    rx.TCD->DLASTSGA = -8;
#else
    rx.transferCount(1);
#endif

    tx.disable();
    tx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI4_TX);
    tx.destination(spi_regs->TCR);
    tx.source(txb);
    tx.transferCount(1);

    // disabled to allow continuous sketch upload 
    // otherwise have to press the button every time to upload anything 
    // rx.enable();
    // tx.enable();

    spi_regs->CR = LPSPI_CR_MEN;
}

static void fourway_write(uint16_t word) {
    static uint8_t const CS2_offset = 2;

    uint32_t low = encode_8_32(word & 0xFF) << CS2_offset,
             high = encode_8_32((word & 0xFF00) >> 8) << CS2_offset;
    while ((spi_regs->FSR & 0xff) > 14)
        ;
    spi_regs->TDR = high;
    spi_regs->TDR = low;
}

void init_ADC() {
    SPI.begin();
    SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE2));
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
    // Turn on sequencer mode; rolling average; average size 2**1=2
    fourway_write((uint16_t)(ADC_WRITE | ADC_CFG1 | ADC_CFG1_SEQ |
                             ADC_CFG1_ROLLING | ADC_CFG1_OSR(3)));
#else
    // Rolling average; average size 2**3=8
    fourway_write(
        (uint16_t)(ADC_WRITE | ADC_CFG1 | ADC_CFG1_ROLLING | ADC_CFG1_OSR(1)));
#endif
    delay(1);
    fourway_write(ADC_NOP);
    fourway_write(ADC_NOP);
    end_fourway();
    delay(10);
}
