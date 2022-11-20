#include "intensity_servo_helper.hpp"
#include <Arduino.h>
#include "read.hpp"
#include "write.hpp"


PowerReading get_best_power(ServoSystem* const sys) {
    PowerReading ret;
    auto * const c = sys->c;
    auto old  = c->reader;
    c->reader = []() { return (double)read_ain0(); };
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
    c->reader = old;
    return ret;
}

auto reference_ic_410_master = 33050., reference_ic_410_slave = 33000.;

PIController ic_410_master = make_iir_cascade_controller(
    []() { return read_ain0() - reference_ic_410_master; },
    [](double x) { write(4, x); },
    {-1 / 3.},
    {0., -1 / 2.},
    -0.015,
    0.,
    950.);

void step_response() {
    static auto ic_410_slave = make_iir_cascade_controller(
        []() { return read_ain1() - reference_ic_410_slave; },
        [](double x) { write(5, x); },
        {-1 / 3.},
        {0., -1 / 2.},
        -0.015,
        0.,
        820.);

    reference_ic_410_master = 33050;
    reference_ic_410_slave  = 32900;
    for (int i = 0; i < 5000; ++i) {
        reference_ic_410_master += 0.116;
        ic_410_master.update();
        ic_410_slave.update();
    }

    reference_ic_410_master = 33630;
    for (int i = 0; i < 5000; ++i) {
        reference_ic_410_master -= 0.116;
        ic_410_master.update();
        ic_410_slave.update();
    }

    reference_ic_410_master = 33050;
    reference_ic_410_slave  = 33450;
    for (int i = 0; i < 5000; ++i) {
        ic_410_master.update();
        ic_410_slave.update();
    }

    reference_ic_410_master = 33630;
    for (int i = 0; i < 5000; ++i) {
        ic_410_master.update();
        ic_410_slave.update();
    }
}

void servo_loop() {
    static auto ic_410_master = make_iir_cascade_controller(
        []() { return read_ain0() - ref_410_master.get_reference(); },
        [](double x) { write(4, x); },
        {-1 / 3.},
        {0., -1 / 2.},
        -0.015,
        0.,
        950.);

    ReferencePath::clear_timer();
    ref_410_master.clear_reference();
    while (!ref_410_master.is_terminated())
        ic_410_master.update();
}
