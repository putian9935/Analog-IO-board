#include "intensity_servo_helper.hpp"
#include <Arduino.h>

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
    ReferencePath::clear_timer();
    for (int ch = 0; ch < 4; ++ch)
        if (mask & (1 << ch))
            servoes[ch].c->reference->clear_reference();
    // as long as one channel does not terminate, the servo goes on
    // the servo will run for at least 2 cycles 
    for (bool end = false; !end; end = true) {
        for (int ch = 0; ch < 4; ++ch)
            if (mask & (1 << ch)) {
                servoes[ch].c->update();
                end &= servoes[ch].c->reference->is_terminated();
            }
    }
    // clean up
    for (int i = 0; i < 10; ++i)
        for (int ch = 0; ch < 4; ++ch)
            if (mask & (1 << ch))
                servoes[ch].c->update();
}
