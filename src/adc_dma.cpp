#include "adc_dma.hpp"
#include <DMAChannel.h> 

static DMAChannel rx, tx;
static uint32_t txb;
volatile uint32_t ch0, ch1; 

void initSPIMasterDMA()
{
    static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI4_S;
    spi_regs->DER = LPSPI_DER_RDDE | LPSPI_DER_TDDE;
    
    rx.begin(true);
    rx.source(spi_regs->RDR);
    rx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI4_RX);
    rx.destination(ch0);
    rx.transferCount(2);
    rx.TCD->DOFF     = 4;
    rx.TCD->DLASTSGA = -8;

    tx.disable();
    tx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI4_TX);
    tx.destination(spi_regs->TCR);
    tx.source(txb);
    tx.transferCount(1);
    txb = spi_regs->TCR | LPSPI_TCR_TXMSK;

    rx.enable();
    tx.enable();
}
