#include "SPISlave.h"

#define SPIS SPI2
#define ChipSelectSlave 44

static IMXRT_LPSPI_t* spis_regs    = &IMXRT_LPSPI1_S;
volatile uint32_t bin0, bin1;
static DMAChannel rx(false);

static void initSPISlaveDMA()
{
    rx.begin(true);
    rx.source(spis_regs->RDR);
    rx.triggerAtHardwareEvent(DMAMUX_SOURCE_LPSPI1_RX);
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
    SPIS.setCS(ChipSelectSlave);

    set_fastio_pin(ChipSelectSlave); 
    set_fastio_pin(45);  // SCK2
    set_fastio_pin(42);  // MISO2

    uint32_t tcr = LPSPI_TCR_FRAMESZ(15);
    if (dataMode & 0x08) tcr |= LPSPI_TCR_CPOL;
    if (dataMode & 0x04) tcr |= LPSPI_TCR_CPHA;
    spis_regs->TCR = tcr;

    spis_regs->CFGR1 = 0;
    spis_regs->DER   = LPSPI_DER_RDDE;
    initSPISlaveDMA();
}


