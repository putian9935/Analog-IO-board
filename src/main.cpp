#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"
#include "pidcontroller.hpp"

void setup()
{
    while (!Serial);
    // Serial.begin(115200);
    init_chips();
}

void loop()
{
  // Serial.printf("%d %d %d %d\n", ain0, ain1, bin0, bin1);

  static PIDController pc(1, 500, 2500, 2500, 800, read_bin0); 

  pc.reference = 2000;
  for(int i = 0; i < 2000; ++i)
    pc.update();

  pc.reference = 0;
  for(int i = 0; i < 2000; ++i)
    pc.update();
}
