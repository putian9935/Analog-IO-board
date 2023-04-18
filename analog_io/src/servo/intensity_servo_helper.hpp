/**
 * @file intensity_servo_helper.hpp
 * @brief Helper functions for intensity servo
 * @date 2023-04-15
 *
 *
 */
#ifndef INTENSITY_SERVO_HELPER
#define INTENSITY_SERVO_HELPER

#include <stdint.h>
#include "controller.hpp"

struct PowerReading {
    int16_t vmin, vmax;
    uint16_t pmin, pmax;
    PowerReading() : vmin(0), vmax(1500), pmin(65535), pmax(0) {}
};

PowerReading get_best_power(Controller* const c, uint16_t const=1);
#endif