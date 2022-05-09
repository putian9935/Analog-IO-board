#include "SPISlave.h"

#define SPIM SPI
#define SPIS SPI2
#define ChipSelectSlave 44

#define SPIS_DMAMUX_SOURCE_RX DMAMUX_SOURCE_LPSPI1_RX
#define CS_MASTER 10
IMXRT_LPSPI_t* spis_regs    = &IMXRT_LPSPI1_S;

volatile uint32_t bin0, bin1;

static DMAChannel rx(false);

static void initSPISlaveDMA()
{
    rx.begin(true);
    rx.source(spis_regs->RDR);
    rx.triggerAtHardwareEvent(SPIS_DMAMUX_SOURCE_RX);
    rx.destination(bin0);
    #ifdef ADC_SEQ_ON
    rx.transferCount(2);
    rx.TCD->DOFF     = 4;
    rx.TCD->DLASTSGA = -8;
    #else 
    rx.transferCount(1);
    #endif 
    rx.enable();
}

void initSPISlave(uint8_t dataMode)
{
    SPIS.begin();
    pinMode(ChipSelectSlave, INPUT);
    SPIS.setCS(ChipSelectSlave);

    uint32_t tcr = LPSPI_TCR_FRAMESZ(15);
    if (dataMode & 0x08) tcr |= LPSPI_TCR_CPOL;
    if (dataMode & 0x04) tcr |= LPSPI_TCR_CPHA;
    
    spis_regs->TCR = tcr;

    spis_regs->CFGR1 = (LPSPI_CFGR1_AUTOPCS | LPSPI_CFGR1_SAMPLE);
    spis_regs->DER   = LPSPI_DER_RDDE;
    spis_regs->CR    = LPSPI_CR_MEN;
    initSPISlaveDMA();
}


