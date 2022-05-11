#include "SPIMaster.h"

static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI4_S;
volatile uint32_t ain0, ain1;

static DMAChannel rx, tx;
static uint32_t txb;

#ifdef COUNT_SAMPLE_RATE
volatile uint32_t cnt_r, cnt_t;

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
    rx.destination(ain0);
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

    rx.enable();
    tx.enable();
}


void initSPIMaster(uint8_t dataMode)
{
    pinMode(10, OUTPUT);
    SPI.begin();
    SPI.beginTransaction(SPISettings(MAX_ADC_FCLK_PRAC, MSBFIRST, dataMode));
    adc_reset();
    delay(1);
    #ifdef ADC_CH0_ON
    // adc_set_ch0();
    #elif defined(ADC_CH1_ON)
    adc_set_ch1();
    #elif defined(ADC_SEQ_ON)
    adc_set_seq();
    #else
    #warning "No ADC channel selection flag defined. Using only CH0. "
    #endif


    SPI.setCS(10);
    
    uint32_t tcr = LPSPI_TCR_FRAMESZ(15);
    if (dataMode & 0x08) tcr |= LPSPI_TCR_CPOL;
    if (dataMode & 0x04) tcr |= LPSPI_TCR_CPHA;
    spi_regs->TCR   = tcr;

    spi_regs->CFGR1 = (LPSPI_CFGR1_SAMPLE | LPSPI_CFGR1_MASTER);
    spi_regs->DER   = LPSPI_DER_RDDE | LPSPI_DER_TDDE;
    spi_regs->CR    = LPSPI_CR_MEN;

    initSPIMasterDMA();
}

