#ifndef INTENSITY_SERVO_HELPER
#define INTENSITY_SERVO_HELPER

#include <stdint.h>
#include "controller.cpp"
#include "reference.hpp"

typedef enum {
    CH1,
    CH2,
    CH3,
    CH4
} Channel;

struct PowerReading {
    uint16_t vmin, vmax, pmin, pmax;

    PowerReading() : vmin(0), vmax(1500), pmin(65535), pmax(0) {}
};

PowerReading get_best_power(Controller* const);
void step_response();
extern PIController ic_410_master;

#endif