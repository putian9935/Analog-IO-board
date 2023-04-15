#include "intensity_servo_helper.hpp"
#include "analog_io.h"


PowerReading get_best_power(Controller* const c) {
    PowerReading ret;
    static ReferencePath ref_old;
    ref_old = *(c->reference);

    *(c->reference) = zero_reference;
    for (uint16_t i = c->lower; i < c->upper; ++i) {
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
    c->writer(c->lower);  // set DAC to lowest after sweep
    *(c->reference) = ref_old;
    return ret;
}
