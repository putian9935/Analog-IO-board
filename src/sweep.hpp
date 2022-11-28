/**
 * @file sweep.hpp
 * @brief SweepController class 
 * @date 2022-11-28
 * 
 * 
 */
#ifndef SWEEP_HPP
#define SWEEP_HPP
#include <stdint.h>

struct SweepController {
    uint16_t lower, upper;
    SweepController() : lower(0), upper(1500) {}
};

#endif