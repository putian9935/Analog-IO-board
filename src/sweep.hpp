#ifndef SWEEP_HPP
#define SWEEP_HPP
#include <stdint.h>

struct SweepController {
    uint16_t lower, upper;
    SweepController() : lower(0), upper(1500) {}
    SweepController(uint16_t lower, uint16_t upper) : lower(lower), upper(upper) {}
};

#endif