#include "SPISlave.h"

#define SPIM SPI
#define SPIS SPI2
#define ChipSelectSlave 44

#define SPIS_DMAMUX_SOURCE_RX DMAMUX_SOURCE_LPSPI1_RX
#define CS_MASTER 10
IMXRT_LPSPI_t* spis_regs    = &IMXRT_LPSPI1_S;
static volatile bool received_data = false;

#define SAMPLES 1
DMAMEM uint16_t static rx_buffer[SAMPLES];

static DMAChannel rx(false);

static void rxISR()
{
    rx.clearInterrupt();
    received_data = true;
    arm_dcache_delete(rx_buffer, SAMPLES); // delete Cache!
}

static void initSPISlaveDMA()
{
    rx.begin(true);
    rx.source((uint16_t&)spis_regs->RDR);
    rx.triggerAtHardwareEvent(SPIS_DMAMUX_SOURCE_RX);
    rx.attachInterrupt(rxISR);
    rx.interruptAtCompletion(); // TCD->CSR |= DMA_TCD_CSR_INTMAJOR;
    rx.destinationBuffer(rx_buffer, SAMPLES + 1);
    rx.enable();
}

void initSPISlave(uint8_t dataMode)
{
    SPIS.begin();
    pinMode(ChipSelectSlave, INPUT);
    SPIS.setCS(ChipSelectSlave);

    spis_regs -> CCR = (spis_regs -> CCR & 0xff);
    uint32_t tcr = LPSPI_TCR_FRAMESZ(15);
    if (dataMode & 0x08) tcr |= LPSPI_TCR_CPOL;
    if (dataMode & 0x04) tcr |= LPSPI_TCR_CPHA;
    tcr |= LPSPI_TCR_TXMSK;
    spis_regs->TCR = tcr;

    spis_regs->CFGR1 = (LPSPI_CFGR1_AUTOPCS | LPSPI_CFGR1_SAMPLE);
    spis_regs->DER   = LPSPI_DER_RDDE;
    spis_regs->CR    = LPSPI_CR_MEN;

    initSPISlaveDMA();
}

void prepare_slave_read() {
    received_data = false; 
}

uint16_t slave_read()
{
    while (!received_data)
        ;
    return rx_buffer[0];
}
