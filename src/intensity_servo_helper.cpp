#include "intensity_servo_helper.hpp"
#include <Arduino.h>
#include "read.hpp"

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
    c->writer(sys->sc.lower); // set DAC to lowest after sweep
    *(c->reference) = ref_old;
    return ret;
}

// void step_response() {
//     static auto ic_410_slave = make_iir_cascade_controller(
//         []() { return read_ain1() - reference_ic_410_slave; },
//         [](double x) { write(5, x); },
//         {-1 / 3.},
//         {0., -1 / 2.},
//         -0.015,
//         0.,
//         820.);

//     reference_ic_410_master = 33050;
//     reference_ic_410_slave  = 32900;
//     for (int i = 0; i < 5000; ++i) {
//         reference_ic_410_master += 0.116;
//         ic_410_master.update();
//         ic_410_slave.update();
//     }

//     reference_ic_410_master = 33630;
//     for (int i = 0; i < 5000; ++i) {
//         reference_ic_410_master -= 0.116;
//         ic_410_master.update();
//         ic_410_slave.update();
//     }

//     reference_ic_410_master = 33050;
//     reference_ic_410_slave  = 33450;
//     for (int i = 0; i < 5000; ++i) {
//         ic_410_master.update();
//         ic_410_slave.update();
//     }

//     reference_ic_410_master = 33630;
//     for (int i = 0; i < 5000; ++i) {
//         ic_410_master.update();
//         ic_410_slave.update();
//     }
// }

void servo_loop(PIController* p) {
    ReferencePath::clear_timer();
    p->reference->clear_reference();
    while (!p->reference->is_terminated())
        p->update();
    for (int i = 0; i < 10; ++i)
        p->update();
}
