/**
 * @file controller.cpp
 * @brief Definition of controllers
 * @date 2022-11-28
 *
 *
 */
#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <Arduino.h>

#include <cmath>
#include <cstring>
#include <vector>

#include "is_int.hpp"
#include "reference.hpp"
#include "serial_reader.hpp"

struct Controller;

enum Command : uint8_t { SWEEP = 1, SERVO, CHANNEL, HSP, SHOW, REF };

typedef double (*read_func_t)();
typedef void (*write_func_t)(double);

// controller base class
struct Controller {
    bool on;
    uint16_t hold_output;
    read_func_t reader;
    write_func_t writer;
    ReferencePath* reference;
    double lower, upper;

    Controller(read_func_t reader, write_func_t writer, ReferencePath* ref,
               double const lower, double const upper)
        : on(false),
          hold_output(0),
          reader(reader),
          writer(writer),
          reference(ref),
          lower(lower),
          upper(upper) {}
    virtual void update() = 0;
    virtual void read_from_serial(uint8_t const) = 0;
    virtual ~Controller(){};
};

#endif  // CONTROLLER_HPP