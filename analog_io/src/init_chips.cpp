#include "init_chips.hpp"
#include <SPI.h>
#include "init_adc.hpp"
#include "pin_assignment.h"
#include "utilities.hpp"

static void init_DAC_spi() {
    SPI1.begin();
    SPI1.beginTransaction(SPISettings(MAX_DAC_FCLK, MSBFIRST, SPI_MODE1));
    SPI1.setCS(CS1);
    SPI1.setMISO(MISO1);

    set_fastio_pin(CS1);
    set_fastio_pin(MISO1);
    set_fastio_pin(MOSI1);
    set_fastio_pin(SCK1);

    IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;

    // clock frequency divisor
    uint16_t div = 720000000 / MAX_DAC_FCLK_PRAC;

#ifdef OVERCLOCK_ON
    spi_regs->CCR                    // clock configuration, p 2882
        = LPSPI_CCR_SCKDIV(div - 2)  // clock freq divisor
          | LPSPI_CCR_DBT(55)        // delay between transfer
          | LPSPI_CCR_PCSSCK(35)     // pcs to sck
          | LPSPI_CCR_SCKPCS(0);     // sck to pcs
#else
    spi_regs->CCR                    // clock configuration, p 2882
        = LPSPI_CCR_SCKDIV(div - 2)  // clock freq divisor
          | LPSPI_CCR_DBT(5)         // delay between transfer
          | LPSPI_CCR_PCSSCK(2)      // pcs to sck
          | LPSPI_CCR_SCKPCS(2);     // sck to pcs
#endif

    spi_regs->TCR                       // transmit command, p 2886
        = (spi_regs->TCR & 0xfffff000)  //
          | LPSPI_TCR_FRAMESZ(47)       // 2x24=48 bits per transfer
          | LPSPI_TCR_RXMSK             // mask away received bits
          | LPSPI_TCR_WIDTH(1);         // two datapins

    spi_regs->CFGR1                // configuration 1, p 2877
        |= LPSPI_CFGR1_PINCFG(1);  // make MISO output
}

static void init_DAC_pins() {
    pinMode(LDAC1, OUTPUT);
    digitalWrite(LDAC1, LOW);
    pinMode(DAC_CLR1, OUTPUT);
    digitalWrite(DAC_CLR1, HIGH);

    pinMode(LDAC2, OUTPUT);
    digitalWrite(LDAC2, LOW);
    pinMode(DAC_CLR2, OUTPUT);
    digitalWrite(DAC_CLR2, HIGH);
}

void init_chips() {
    // increase CPU clock speed to 720MHz
    set_arm_clock_cpp(720000000);

    init_DAC_pins();

    init_DAC_spi();

    init_ADC();
    // calibration of dac starts from here

    delay(200);
}
