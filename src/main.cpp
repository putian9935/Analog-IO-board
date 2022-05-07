


#include "init_chips.hpp"
#include "utilities.hpp"
#include "read.hpp"
#include "write.hpp"


void setup()
{
    while (!Serial);
    Serial.begin(115200);
    
    init_chips();
    
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);
    
    // // adc_set_ch0();
    // adc_set_seq();
    delay(1);
    // ADC_TRANSFER_NOP; 
    // ADC_TRANSFER_NOP;

}

elapsedMicros t; 
int loop_count = 0;

int cycle(uint16_t num) {
    IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;
    IMXRT_LPSPI_t* spim_regs = &IMXRT_LPSPI4_S;
    spi_regs->TDR = ((((uint32_t)((2 & 3) | DAC_DATA_REG)) << 16)+ (uint32_t)num); 
    spim_regs ->TDR = 0; 
    while ((spim_regs->RSR & LPSPI_RSR_RXEMPTY));
    int ret = spim_regs -> RDR;
    while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY));
    spi_regs -> RDR;
    return ret;
}

  IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI3_S;
  IMXRT_LPSPI_t* spim_regs = &IMXRT_LPSPI4_S;

void follower() {
  
    // delayNanoseconds(200);
    // ASSERT_ADC;
    // delayNanoseconds(200);    
    ASSERT_ADC;
    delayNanoseconds(200);
    DEASSERT_ADC;
    ASSERT_ADC;
    spim_regs ->TDR = 0; 
    ASSERT_DAC1;
    while ((spim_regs->RSR & LPSPI_RSR_RXEMPTY));
    spi_regs->TDR = ((((uint32_t)((2 & 3) | DAC_DATA_REG)) << 16)+ (uint32_t)(spim_regs -> RDR >> 2)); 
    
    DEASSERT_ADC; 
    
    // delayNanoseconds(50);
    while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY));
    spi_regs -> RDR;
    
    DEASSERT_DAC1;
    // return ret;
}

uint16_t follower2(uint16_t data) {
    ASSERT_DAC1;
    spi_regs->TDR = ((((uint32_t)((2 & 3) | DAC_DATA_REG)) << 16)+ (uint32_t)(data)); 
    ASSERT_ADC;
    spim_regs ->TDR = 0; 
    while ((spim_regs->RSR & LPSPI_RSR_RXEMPTY));
    spim_regs -> RDR;
    DEASSERT_ADC;
    ASSERT_ADC;
    spim_regs ->TDR = 0; 
    while ((spim_regs->RSR & LPSPI_RSR_RXEMPTY));
    uint16_t ret  = spim_regs -> RDR;
    DEASSERT_ADC;
    while ((spi_regs->RSR & LPSPI_RSR_RXEMPTY));
    spi_regs -> RDR;
    DEASSERT_DAC1;
    return ret;
}

uint16_t last_read; 
void loop()
{
  // for(int i = 0 ; i < 90; ++i) {
    // last_read = follower2(last_read >> 2);
    // delayNanoseconds(10);
    // write(2, (i << 6));
    // delayNanoseconds(1000);
    // write(6, (i << 6));
    // write(2, (i << 6));
    // write(6, (i << 6));
    // read_high_throughput();
    // Serial.println(read_high_throughput());
    // read_quad_high_throughput();
  // }
  uint32_t cnt = 0;
  while (t < 1000000){
    // write(2,2);
    // read_high_throughput();
    last_read = follower2(last_read >> 2);
    // follower();
    ++cnt;
  }
  Serial.println(cnt); 
  t = 0;

  // prepare_slave_read();
  
  // ASSERT_ADC;
  // transfer16(loop_count);
  // Serial.printf("SPI Transfer: %x  SPI DMA: %x \n", loop_count, slave_read());
  // DEASSERT_ADC;
  
  // delay(250);
  // loop_count++;
}


