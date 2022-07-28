#include "clockspeed.h"
#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"
#include "pidcontroller.hpp"
void setup()
{
    // This will set a higher IPG root clock
    CCM_CBCDR = (CCM_CBCDR & ~CCM_CBCDR_IPG_PODF_MASK) | CCM_CBCDR_IPG_PODF(1);
    while (!Serial);
    // Serial.begin(115200);
    // set_arm_clock_cpp(800000000);
    init_chips();
    delay(200);
}

void loop()
{
  // Serial.printf("%d %d %d %d\n", ain0, bin0, ain1, bin1);
  static PIDController pc(1, 800, 800, 150, 4000, read_bin0); 


  pc.reference = 2000;
  for(int i = 0; i < 2000; ++i)
  {
    pc.update();
  }

  // pc.reference = 0;
  for(int i = 0; i < 2000; ++i)
    pc.update();
}
