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

    uint16_t div = 264000000 / MAX_DAC_FCLK_PRAC;

    // clock frequency divisor
    spi_regs->CCR                    // clock configuration, p 2882
        = LPSPI_CCR_SCKDIV(0)  // clock freq divisor
          | LPSPI_CCR_DBT(0)         // delay between transfer
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
        auto x = read_ain0();
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

/**
 * @brief Test correctness and crosstalk of ADC 
 * 
 * Setup: 
 * 
 * 1. connect 0.25Hz 10Vpp square wave to one of the ADC channel (AIN0, say); 
 * 2. observe if the corresponding channel (AIN1) has different results; 
 * 3. if the results differ between alternating states, increase DBT of SPI CCR register. 
 */
// void test3() {
//     delay(1000);
//     Serial.printf("%u %u %u %u\n", read_ain0(), read_bin0(),read_ain1(), read_bin1());
// }

/**
 * @brief Bandwidth as a unit follower 
 * 
 */
void test4() {
    while (1) {
        auto x = read_ain0();
        spi_regs->TDR = ((0x10u << 16) | x);
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;
        read_bin0();
        spi_regs->TDR = ((0x12u << 16) | x);
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;
        read_bin0();
        spi_regs->TDR = ((0x14u << 16) | x);
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;
    }
}


void test5(double gain) {
    // one/two channel 1.3; three channel 1.2 

    while (1) {
        int16_t x = read_ain0();
        int16_t err = (x - 32768) ; 
        spi_regs->TDR = ((0x10u << 16) | ((int16_t)(-(double)(err) * gain) + 32768));
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;

        read_ain0(); 
        spi_regs->TDR = ((0x12u << 16) | x);
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;
        read_ain0();
        spi_regs->TDR = ((0x14u << 16) | x);
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;    
        read_ain0();
        spi_regs->TDR = ((0x14u << 16) | x);
        while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY))
            ;
        spi_regs->RDR;      
    }
}

void loop() {
    // test1();
    // test2();
    // test3();
    test5(1.3);

    // while ((spi_regs->FSR & 0xff) > 14)
    //     ;
    // spi_regs->TDR = (0x100100);
}
