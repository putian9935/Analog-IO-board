
#include "init_chips.hpp"
#include "utilities.hpp"
#include "read.hpp"
#include "write.hpp"

void setup()
{
    while (!Serial);
    Serial.begin(115200);
    init_chips();
    adc_set_ch1();
    // adc_set_seq();
    delay(1);
    ADC_TRANSFER_NOP; 
    ADC_TRANSFER_NOP;
    // adc_set_seq();
}

elapsedMicros t;
void loop()
{
  
  // uint16_t x = read_high_throughput();
  // Serial.println(x); 
  // pair p = read_pair_high_throughput();
  // Serial.println(p.a);
  // Serial.println(p.b);
  


    uint64_t cnt = 0;
    uint32_t m = 0;
    uint32_t mi = 40000;
    uint32_t pa_max = 0, pa_min = 40000, pb_max = 0, pb_min = 40000;
    while (t < 1000000)
    {
      
      // write(0, 2);
      // uint16_t x = read_high_throughput();
      pair p = read_pair_high_throughput();
      // m = (m > x)?m:x;
      // mi = min(mi, x);
      // pa_max = max(pa_max, p.a);
      // pa_min = min(pa_min, p.a);
      // pb_max = max(pb_max, p.b);
      // pb_min = min(pb_min, p.b);
      ++cnt;
    }
    
    t = 0;
    Serial.println(cnt);
    Serial.print(pa_max); Serial.print(' '); Serial.print(pb_max); Serial.print(' ');Serial.println(m);
     Serial.print(pa_min); Serial.print(' '); Serial.print(pb_min); Serial.print(' ');Serial.println(mi);
     


}

