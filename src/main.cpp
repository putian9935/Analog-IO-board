#include "analog_io.h"
#include "pin_assignment.h"
#include "SPI.h"
#include "init_adc.hpp"

// #include "fsm.hpp"
// #include "trigger.h"

#define DAC_LDAC 24
#define DAC_ENABLE 25
static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;

#include "utilities.hpp"

void init_DAC() {
    
    pinMode(DAC_LDAC, OUTPUT);
    digitalWrite(DAC_LDAC, LOW);
    pinMode(DAC_ENABLE, OUTPUT);
    digitalWrite(DAC_ENABLE, LOW);

    SPI1.begin();
    SPI1.beginTransaction(SPISettings(120000000, MSBFIRST, SPI_MODE1));
    SPI1.setCS(CS1);
    SPI1.setMISO(MISO1);

    set_fastio_pin(CS1);
    set_fastio_pin(MISO1);
    set_fastio_pin(MOSI1);
    set_fastio_pin(SCK1);

    uint16_t div = 720000000 / MAX_DAC_FCLK_PRAC;

    // clock frequency divisor
    spi_regs->CCR                    // clock configuration, p 2882
        = LPSPI_CCR_SCKDIV(div - 2)  // clock freq divisor
          | LPSPI_CCR_DBT(1)         // delay between transfer
          | LPSPI_CCR_PCSSCK(1)      // pcs to sck
          | LPSPI_CCR_SCKPCS(0);     // sck to pcs

    spi_regs->TCR                       // transmit command, p 2886
        = (spi_regs->TCR & 0xfffff000)  //
          | LPSPI_TCR_FRAMESZ(23);             // mask away received bits
}
void setup() {
    set_arm_clock_cpp(720000000);
    while (!Serial)
    ;
    Serial.begin(115200);

    init_ADC();
    init_DAC();
    // init_trigger();
    // init_chips();
    delay(200);
    // pinMode(CS1, OUTPUT);

    pinMode(40, OUTPUT);
}

// void write(uint16_t value) {
    
// }

void loop() {

    while (1) {
        auto x = read_ain0(); 
        digitalWriteFast(40, x > 31500);
    spi_regs->TDR = ((0x10u << 16) | x);
    while((spi_regs->RSR & LPSPI_RSR_RXEMPTY));
    spi_regs->RDR;
    }
    // while ((spi_regs->FSR & 0xff) > 14)
    //     ;
    // spi_regs->TDR = (0x100100);
    // delay(1000);
    // Serial.printf("%u %u %u %u\n", read_ain0(), read_ain1(), read_bin0(), read_bin1());
    // Serial.printf("%u %u %u %u\n", read_ain0(), read_ain1(), read_bin0(), read_bin1());

}
