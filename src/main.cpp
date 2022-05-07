


#include "init_chips.hpp"
#include "utilities.hpp"
#include "read.hpp"
#include "write.hpp"


void setup()
{
    while (!Serial);
    Serial.begin(115200);
    
    init_chips();
    
    
    // // adc_set_ch0();
    // adc_set_seq();
    // delay(1);
    // ADC_TRANSFER_NOP; 
    // ADC_TRANSFER_NOP;

}

elapsedMicros t; 
int loop_count = 0;
void loop()
{
  // for(int i = 0 ; i < 90; ++i) {

  //   write(2, (i << 6));
  //   write(6, (i << 6));
  //   write(2, (i << 6));
  //   write(6, (i << 6));
  //   read_quad_high_throughput();
  // }
  // uint32_t cnt = 0;
  // while (t < 1000000){
  //   // write(2,2);
  //   // read_high_throughput();
  //   ++cnt;
  // }
  // Serial.println(cnt); 
  // t = 0;

  prepare_slave_read();
  
  ASSERT_ADC;
  transfer16(loop_count);
  Serial.printf("SPI Transfer: %x  SPI DMA: %x \n", loop_count, slave_read());
  DEASSERT_ADC;
  
  delay(250);
  loop_count++;
}


