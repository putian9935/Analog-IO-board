#ifndef SERVO_SYSTEM_H
#define SERVO_SYSTEM_H

#include <memory>
#include "controller.cpp"
#include "reference.hpp"
#include "sweep.hpp"

struct ServoSystem {
    Controller * const c;
    SweepController sc;

    template <int len_zeroes, int len_poles>
    ServoSystem(IIRCascadeController<len_zeroes, len_poles> *pc) : 
        c(pc),
        sc() {}
};

extern ServoSystem master_410_servo;

#endif 