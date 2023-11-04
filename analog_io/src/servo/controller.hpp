/**
 * @file controller.cpp
 * @brief Definition of controllers
 * @date 2023-11-4
 *
 *
 */
#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "is_int.hpp"
#include "reference.hpp"
#include "serial_reader.hpp"
#include <Arduino.h>
#include <cmath>
#include <cstring>
#include <vector>

struct Controller;

enum Command : uint8_t {
    SWEEP = 1,
    SERVO,
    CHANNEL,
    HSP,
    SHOW,
    REF
};

typedef float (*read_func_t)();
typedef void (*write_func_t)(uint16_t);

// controller base class
struct Controller {
    bool on;
    // uint16_t hold_output;
    read_func_t reader;
    write_func_t writer;
    ReferencePath* reference;
    ReferencePath* reference_hsp;
    float lower, upper;

    Controller(read_func_t reader, write_func_t writer, ReferencePath* ref, ReferencePath* ref_hsp,
               double const lower, double const upper)
        : on(false),
          reader(reader),
          writer(writer),
          reference(ref),
          reference_hsp(ref_hsp),
          lower(lower),
          upper(upper) {}
    // loop update
    virtual void update() = 0;
    // update parameter from serial
    virtual void read_from_serial(uint8_t const) = 0;
    virtual void clear() = 0;
    virtual ~Controller(){};
};

typedef Controller* const pController;

#endif // CONTROLLER_HPP