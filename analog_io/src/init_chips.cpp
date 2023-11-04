#include "init_chips.hpp"

#include <SPI.h>

#include "init_adc.hpp"
#include "pin_assignment.h"
#include "utilities.hpp"

static void init_DAC() {
    IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;
    pinMode(DAC_LDAC, OUTPUT);
    digitalWrite(DAC_LDAC, LOW);
    pinMode(DAC_ENABLE, OUTPUT);
    digitalWrite(DAC_ENABLE, LOW);

    SPI1.begin();
    SPI1.beginTransaction(SPISettings(MAX_DAC_FCLK_PRAC, MSBFIRST, SPI_MODE1));
    SPI1.setCS(CS1);
    SPI1.setMISO(MISO1);

    set_fastio_pin(CS1);
    set_fastio_pin(MISO1);
    set_fastio_pin(MOSI1);
    set_fastio_pin(SCK1);

    // clock frequency divisor
    spi_regs->CCR                       // clock configuration, p 2882
        = LPSPI_CCR_SCKDIV(0)           // clock freq divisor
          | LPSPI_CCR_DBT(0)            // delay between transfer
          | LPSPI_CCR_PCSSCK(1)         // pcs to sck
          | LPSPI_CCR_SCKPCS(0);        // sck to pcs
    spi_regs->TCR                       // transmit command, p 2886
        = (spi_regs->TCR & 0xfffff000)  //
          | LPSPI_TCR_FRAMESZ(23);      // mask away received bits

    // turn off internal reference 
    spi_regs->TDR = (0x012000);
    while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
        ;
    spi_regs->RDR;
    // all output to zero 
    spi_regs->TDR = (0x348000);
    while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
        ;
    // the readback is explicitly commented out, and this greatly 
    // improves the performance. Correspondingly, async_write function 
    // must be used consistently throughout the program. Otherwise, 
    // instant crash is expected. 

    // spi_regs->RDR;
}

void init_chips() {
    // increase CPU clock speed to 720MHz
    // set_arm_clock_cpp(720000000);
    // CCM_CBCDR = ((CCM_CBCDR & ~CCM_CBCDR_IPG_PODF_MASK) | CCM_CBCDR_IPG_PODF(2));

    init_DAC();
    init_ADC();
    // calibration of dac starts from here

    delay(200);
}
