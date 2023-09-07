#include "SPI.h"
#include "analog_io.h"
#include "init_adc.hpp"
#include "pin_assignment.h"

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
    SPI1.beginTransaction(SPISettings(MAX_DAC_FCLK_PRAC, MSBFIRST, SPI_MODE1));
    SPI1.setCS(CS1);
    SPI1.setMISO(MISO1);

    set_fastio_pin(CS1);
    set_fastio_pin(MISO1);
    set_fastio_pin(MOSI1);
    set_fastio_pin(SCK1);

    uint16_t div = 720000000 / MAX_DAC_FCLK_PRAC;

    // // clock frequency divisor, single channel
    // spi_regs->CCR                    // clock configuration, p 2882
    //     = LPSPI_CCR_SCKDIV(div - 2)  // clock freq divisor
    //       | LPSPI_CCR_DBT(1)         // delay between transfer
    //       | LPSPI_CCR_PCSSCK(1)      // pcs to sck
    //       | LPSPI_CCR_SCKPCS(0);     // sck to pcs

    // clock frequency divisor
    spi_regs->CCR                    // clock configuration, p 2882
        = LPSPI_CCR_SCKDIV(div - 2)  // clock freq divisor
          | LPSPI_CCR_DBT(1)         // delay between transfer
          | LPSPI_CCR_PCSSCK(1)      // pcs to sck
          | LPSPI_CCR_SCKPCS(0);     // sck to pcs
    spi_regs->TCR                       // transmit command, p 2886
        = (spi_regs->TCR & 0xfffff000)  //
          | LPSPI_TCR_FRAMESZ(23);      // mask away received bits
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

extern volatile uint32_t ain0, ain1;

#include "bit_mangler.h"

#ifdef ADC_SINGLE_CHANNEL
uint16_t read_ain0() {
    return decode_32_16(ain0 >> 1);
}
#endif

inline void test1() {
    // delay test
    while (1) {
        // Serial.printf("%u %u\n", decode_32_16(ain0>>1), decode_32_16(ain0));
        // delay(500);
        auto x = decode_32_16(ain0 >> 1);
        digitalWriteFast(40, x > 31500);
        spi_regs->TDR = ((0x10u << 16) | x);
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;
    }
}

inline void test2() {
    // write speed 28.6Hz x 65535 = 1.9MHz
    while (1) {
        for (int x = 0; x < 65535; ++x) {
            spi_regs->TDR = ((0x10u << 16) | x);
            while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
                ;
            spi_regs->RDR;
        }
    }
}

void test3() {
    delay(1000);
    Serial.printf("%u %u %u %u\n", decode_32_16(ain0 >> 1), decode_32_16(ain0), decode_32_16(ain1 >> 1), decode_32_16(ain1));
}

void test4() {
    while (1) {
        auto x = decode_32_16(ain0 >> 1);
        spi_regs->TDR = ((0x10u << 16) | x);
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;
        decode_32_16(ain0);
        spi_regs->TDR = ((0x12u << 16) | x);
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;
        decode_32_16(ain0);
        spi_regs->TDR = ((0x14u << 16) | x);
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;
    }
}

int16_t twosCompToDec(uint16_t two_compliment_val)
{
    // [0x0000; 0x7FFF] corresponds to [0; 32,767]
    // [0x8000; 0xFFFF] corresponds to [-32,768; -1]
    // int16_t has the range [-32,768; 32,767]

    uint16_t sign_mask = 0x8000;

    // if positive
    if ( (two_compliment_val & sign_mask) == 0 ) {
        return two_compliment_val;
    //  if negative
    } else {
        // invert all bits, add one, and make negative
        return two_compliment_val - 32768;
    }
}

void test5(double gain) {
    // one/two channel 1.3; three channel 1.2 

    while (1) {
        int16_t x = decode_32_16(ain0 >> 1);
        int16_t err = (x - 32768) ; 
        spi_regs->TDR = ((0x10u << 16) | ((int16_t)(-(double)(err) * gain) + 32768));
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;


        decode_32_16(ain0);
        spi_regs->TDR = ((0x12u << 16) | x);
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;
        decode_32_16(ain0);
        spi_regs->TDR = ((0x14u << 16) | x);
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;        
    }
}

void loop() {
    test5(1.2);

    // while ((spi_regs->FSR & 0xff) > 14)
    //     ;
    // spi_regs->TDR = (0x100100);
}
