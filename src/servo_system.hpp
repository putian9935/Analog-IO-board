#ifndef SERVO_SYSTEM_H
#define SERVO_SYSTEM_H

#include "controller.cpp"
#include "reference.hpp"
#include "sweep.hpp"


template <int len_zeroes, int len_poles>
struct ServoSystem {
    using ControllerType = IIRCascadeController<len_zeroes, len_poles>;
    ControllerType * const c;
    SweepController sc;

    ServoSystem(ControllerType *pc) : 
        c(pc),
        sc() {}
};

using PIServoSystem = ServoSystem<1, 2>;
extern PIServoSystem master_410_servo;
extern ReferencePath ref_410_master;

#endif 