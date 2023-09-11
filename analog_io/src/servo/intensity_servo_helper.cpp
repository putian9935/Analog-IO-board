#include "intensity_servo_helper.hpp"
#include "analog_io.h"


PowerReading get_best_power(Controller* const c, uint16_t const step) {
    PowerReading ret;
    static ReferencePath ref_old;
    ref_old = *(c->reference);

    *(c->reference) = zero_reference;
    c->writer(c->lower);
    delay(5);
    for (auto i = c->lower; i < c->upper; i += step) {
        c->writer(i);
        delayMicroseconds(500);
        auto x = c->reader();
        if (x > ret.pmax) {
            ret.pmax = x;
            ret.vmax = i;
        }
        if (x < ret.pmin) {
            ret.pmin = x;
            ret.vmin = i;
        }
        if(Serial.available()) {
            break;
        }
    }
    c->writer(c->lower);  // set DAC to lowest after sweep
    *(c->reference) = ref_old;
    return ret;
}
