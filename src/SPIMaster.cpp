#include "SPIMaster.h"

#include <DMAChannel.h>
#include <SPI.h>
#include "init_chips.hpp"
#include "utilities.hpp"

#ifdef COUNT_SAMPLE_RATE 
volatile uint32_t cnt_r, cnt_t;
#endif 

static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI4_S;
volatile uint32_t ain0, ain1;

static DMAChannel rx, tx;
static uint32_t txb;

#ifdef COUNT_SAMPLE_RATE
static void rx_isr()
{
    rx.clearInterrupt();
    asm volatile ("dsb");
    cnt_r += 2;
}

static void tx_isr()
{
    tx.clearInterrupt();
    asm volatile ("dsb");
    ++cnt_t;
}
#endif 


static void initSPIMasterDMA()
{
    rx.begin(true);
    rx.source(spi_regs->RDR);
    rx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI4_RX);
    #ifdef ADC_CH1_ON
    rx.destination(ain1);
    #else
    rx.destination(ain0);
    #endif // ADC_CH1_ON 
    #ifdef COUNT_SAMPLE_RATE
    rx.attachInterrupt(rx_isr);
    rx.interruptAtCompletion();
    #endif 
    #ifdef ADC_SEQ_ON
    rx.transferCount(2);
    rx.TCD->DOFF     = 4;
    rx.TCD->DLASTSGA = -8;
    #else 
    rx.transferCount(1);
    #endif 

    tx.disable();
    tx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI4_TX);
    tx.destination(spi_regs->TDR);
    tx.source(txb);
    tx.transferCount(1);
    
    #ifdef COUNT_SAMPLE_RATE
    tx.attachInterrupt(tx_isr);
    tx.interruptAtCompletion();
    #endif
    txb = 0;

    CCM_CBCMR = (CCM_CBCMR & ~(CCM_CBCMR_LPSPI_PODF_MASK | CCM_CBCMR_LPSPI_CLK_SEL_MASK)) |
		CCM_CBCMR_LPSPI_PODF(0) | CCM_CBCMR_LPSPI_CLK_SEL(1);

    rx.enable();
    tx.enable();
}


void initSPIMaster(uint8_t dataMode)
{
    pinMode(10, OUTPUT);
    SPI.begin();
    SPI.beginTransaction(SPISettings(ADC_FCLK, MSBFIRST, dataMode));
    SPI.setCS(10);

    set_fastio_pin(CS); 
    set_fastio_pin(11);  // MOSI
    set_fastio_pin(12);  // MISO
    set_fastio_pin(13);  // SCK

    adc_reset();
    #ifdef ADC_CH0_ON
    // adc_set_ch0();
    #elif defined(ADC_CH1_ON)
    adc_set_ch1();
    #elif defined(ADC_SEQ_ON)
    adc_set_seq();
    #else
    #warning "No ADC channel selection flag defined. Using only CH0. "
    #endif
    
    uint16_t div = 720000000 / ADC_FCLK;
    #ifndef ADC_SEQ_ON
    spi_regs -> CCR = LPSPI_CCR_SCKDIV(div-2) | LPSPI_CCR_DBT(4) | LPSPI_CCR_PCSSCK(3) | LPSPI_CCR_SCKPCS(3);
    #else
    spi_regs -> CCR = LPSPI_CCR_SCKDIV(div-2) | LPSPI_CCR_DBT(255) | LPSPI_CCR_PCSSCK(255) | LPSPI_CCR_SCKPCS(255);
    #endif

    uint32_t tcr = LPSPI_TCR_FRAMESZ(15);
    if (dataMode & 0x08) tcr |= LPSPI_TCR_CPOL;
    if (dataMode & 0x04) tcr |= LPSPI_TCR_CPHA;
    spi_regs->TCR   = tcr;

    spi_regs->CFGR1 = (LPSPI_CFGR1_SAMPLE | LPSPI_CFGR1_MASTER);
    spi_regs->DER   = LPSPI_DER_RDDE | LPSPI_DER_TDDE;

    initSPIMasterDMA();
    delay(1);
    spi_regs->CR    = LPSPI_CR_MEN;
}

