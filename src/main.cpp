#include "analog_io.h"
#include "pin_assignment.h"
#include "SPI.h"
// #include "fsm.hpp"
// #include "trigger.h"

#define DAC_LDAC 24
#define DAC_ENABLE 25
static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;

void setup() {
    pinMode(DAC_LDAC, OUTPUT);
    digitalWrite(DAC_LDAC, LOW);
    pinMode(DAC_ENABLE, OUTPUT);
    digitalWrite(DAC_ENABLE, LOW);

    SPI1.begin();
    SPI1.beginTransaction(SPISettings(72000000, MSBFIRST, SPI_MODE1));
    SPI1.setCS(CS1);
    SPI1.setMISO(MISO1);

    set_fastio_pin(CS1);
    set_fastio_pin(MISO1);
    set_fastio_pin(MOSI1);
    set_fastio_pin(SCK1);


    // clock frequency divisor

    spi_regs->TCR                       // transmit command, p 2886
        = (spi_regs->TCR & 0xfffff000)  //
          | LPSPI_TCR_FRAMESZ(23)
          | LPSPI_TCR_RXMSK;             // mask away received bits

    // while (!Serial)
    // ;
    // Serial.begin(115200);
    // init_trigger();
    // init_chips();

    // pinMode(CS1, OUTPUT);
}



void loop() {
    while ((spi_regs->FSR & 0xff) > 14)
        ;
    spi_regs->TDR = (0x107FFF);
    delayMicroseconds(20);
    while ((spi_regs->FSR & 0xff) > 14)
        ;
    spi_regs->TDR = (0x108100);
    delayMicroseconds(20);

}
