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
#include "fsm.hpp"
#include "serial_command.hpp"

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

    Controller(
        read_func_t reader,
        write_func_t writer,
        ReferencePath* ref,
        double const lower,
        double const upper)
        : on(false),
          hold_output(0),
          reader(reader),
          writer(writer),
          reference(ref),
          lower(lower),
          upper(upper) {}
    virtual void update()                        = 0;
    virtual void read_from_serial(uint8_t const) = 0;
    virtual ~Controller(){};
};

// first order IIR controller base class
struct IIRBaseController {
    double le, lo;  // last error and last output
    IIRBaseController() : le{}, lo{} {}
    virtual double transfer(double const err) = 0;
    virtual void show() = 0;
    virtual ~IIRBaseController(){};
};

// first order IIR controller with zero and pole
struct IIRFirstOrderController : public IIRBaseController {
    double const z, p;
    IIRFirstOrderController(double const z, double const p) : z(z), p(p) {}
    double transfer(double const e) override {
        double ret = ((1 + M_PI * p) * lo - (1 + M_PI * z) * le + (1 - M_PI * z) * e) / (1 - M_PI * p);
        lo         = ret;
        le         = e;
        return ret;
    }
    void show() override {
        Serial.printf("Zero: %.3f; Pole: %.3f\n", z, p);
    }
};

// first order IIR controller with only pole
struct IIRSinglePoleController : public IIRBaseController {
    double const p;
    IIRSinglePoleController(double const p) : p(p) {}
    double transfer(double const e) override {
        double ret = ((1 + M_PI * p) * lo + M_PI * (le + e)) / (1 - M_PI * p);
        lo         = ret;
        le         = e;
        return ret;
    }    
    void show() override {
        Serial.printf("Pole: %.3f\n", p);
    }
};

// IIR controller in cascade form
template <int len_zeroes, int len_poles>
struct IIRCascadeController : public Controller {
    static_assert(len_zeroes < len_poles, "There should be more poles than zeroes. ");

    double overall_gain;
    double last_out;
    std::vector<IIRBaseController*> controllers;

    IIRCascadeController(
        read_func_t reader, write_func_t writer,
        double const (&zeroes)[len_zeroes],
        double const (&poles)[len_poles],
        double const overall_gain,
        double const lower, double const upper,
        ReferencePath* reference)
        : Controller(reader, writer, reference, lower, upper),
          overall_gain(overall_gain),
          last_out(0.) {
        int i = 0;
        for (; i < len_zeroes; ++i)
            controllers.push_back(new IIRFirstOrderController(zeroes[i], poles[i]));
        for (; i < len_poles; ++i)
            controllers.push_back(new IIRSinglePoleController(poles[i]));
    }

    void update() override {
        double new_out = reader() - reference->get_reference();  // start with an error
        for (auto p : controllers)
            new_out = p->transfer(new_out);
        new_out = max(min(overall_gain * new_out, upper), lower);
        if (abs(new_out - last_out) > 1.)  // lazy update
        {
            writer(int(new_out));
            last_out = (int)new_out;
        }
    }
    void read_from_serial(uint8_t const) override;
};

template <int a, int b>
void __attribute__((weak)) IIRCascadeController<a, b>::read_from_serial(uint8_t const c)  {
    switch (c) {
        case SWEEP : {
            sweep_parser(this);
            break;
        }
        case SERVO: {
            servo_parser(this);
            break;
        }
        case SHOW: {
            show_parser(this);
            break;
        }
    }
}

// wrapper for constructors
template <int len_zeroes, int len_poles, typename T, typename U>
IIRCascadeController<len_zeroes, len_poles> make_iir_cascade_controller(read_func_t reader, write_func_t writer, T const (&zeroes)[len_zeroes], U const (&poles)[len_poles], double const overall_gain, double const lower = -32768., double const upper = 32767., ReferencePath* reference = &zero_reference) {
    static_assert(!is_int<T>::value, "Initialization error detected. Did you forget to put decimal point?\n(e.g. writing -1/3 instead of -1./3");
    static_assert(!is_int<U>::value, "Initialization error detected. Did you forget to put decimal point?\n(e.g. writing -1/3 instead of -1./3");
    return IIRCascadeController<len_zeroes, len_poles>(reader, writer, zeroes, poles, overall_gain, lower, upper, reference);
}

#endif  // CONTROLLER_HPP