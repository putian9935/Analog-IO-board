#ifndef INTENSITY_SERVO_HELPER
#define INTENSITY_SERVO_HELPER

#include <stdint.h>
#include "servo_system.hpp"

struct PowerReading {
    uint16_t vmin, vmax, pmin, pmax;
    PowerReading() : vmin(0), vmax(1500), pmin(65535), pmax(0) {}
};

PowerReading get_best_power(PIServoSystem* const sys);

void servo_loop(uint8_t const mask);
#endif