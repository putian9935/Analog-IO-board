#include "intensity_servo_helper.hpp"
#include <Arduino.h>
#include "pin_assignment.h"
#include "write.hpp" 

uint16_t hold_output[4] = {0}; 

PowerReading get_best_power(PIServoSystem* const sys) {
    PowerReading ret;
    auto* const c = sys->c;
    static ReferencePath ref_old;
    ref_old = *(c->reference);

    *(c->reference) = zero_reference;
    for (uint16_t i = sys->sc.lower; i < sys->sc.upper; ++i) {
        c->writer(i);
        delay(1);
        auto x = c->reader();
        if (x > ret.pmax) {
            ret.pmax = x;
            ret.vmax = i;
        }
        if (x < ret.pmin) {
            ret.pmin = x;
            ret.vmin = i;
        }
    }
    c->writer(sys->sc.lower);  // set DAC to lowest after sweep
    *(c->reference) = ref_old;
    return ret;
}

void servo_loop(uint8_t const mask) {
    if (digitalReadFast(GLOBAL_ENABLE_PIN)){ 
      for (int ch = 0; ch < 4; ++ch)
          if (mask & (1 << ch)) 
            write(ch + 4, hold_output[ch]); 
        return;
    }
     
    for (int ch = 0; ch < 4; ++ch)
        if (mask & (1 << ch)) 
            servoes[ch].c->update();
}
