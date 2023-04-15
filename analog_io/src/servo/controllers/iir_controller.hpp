#ifndef IIR_CONTROLLER
#define IIR_CONTROLLER

#include "servo/controller.hpp"
#include "servo/intensity_servo_helper.hpp"

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
        double ret =
            ((1 + M_PI * p) * lo - (1 + M_PI * z) * le + (1 - M_PI * z) * e) /
            (1 - M_PI * p);
        lo = ret;
        le = e;
        return ret;
    }
    void show() override { Serial.printf("Zero: %.3f; Pole: %.3f\n", z, p); }
};

// first order IIR controller with only pole
struct IIRSinglePoleController : public IIRBaseController {
    double const p;
    IIRSinglePoleController(double const p) : p(p) {}
    double transfer(double const e) override {
        double ret = ((1 + M_PI * p) * lo + M_PI * (le + e)) / (1 - M_PI * p);
        lo = ret;
        le = e;
        return ret;
    }
    void show() override { Serial.printf("Pole: %.3f\n", p); }
};

// IIR controller in cascade form
template <int len_zeroes, int len_poles>
struct IIRCascadeController : public Controller {
    static_assert(len_zeroes < len_poles,
                  "There should be more poles than zeroes. ");

    double overall_gain;
    double last_out;
    std::vector<IIRBaseController*> controllers;

    IIRCascadeController(read_func_t reader, write_func_t writer,
                         double const (&zeroes)[len_zeroes],
                         double const (&poles)[len_poles],
                         double const overall_gain, double const lower,
                         double const upper, ReferencePath* reference)
        : Controller(reader, writer, reference, lower, upper),
          overall_gain(overall_gain),
          last_out(0.) {
        int i = 0;
        for (; i < len_zeroes; ++i)
            controllers.push_back(
                new IIRFirstOrderController(zeroes[i], poles[i]));
        for (; i < len_poles; ++i)
            controllers.push_back(new IIRSinglePoleController(poles[i]));
    }

    void update() override {
        double new_out =
            reader() - reference->get_reference();  // start with an error
        for (auto p : controllers) new_out = p->transfer(new_out);
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
void sweep_parser(IIRCascadeController<a, b>* c) {
    c->lower = (uint16_t)SerialReader();
    c->upper = (uint16_t)SerialReader();
    auto gp = get_best_power(c);
    Serial.printf("Max power of %d at DAC number %d.\n", gp.pmax, gp.vmax);
    Serial.printf("Min power of %d at DAC number %d.\n", gp.pmin, gp.vmin);
}

template <int a, int b>
void show_parser(IIRCascadeController<a, b>* c) {
    Serial.printf("Lower: %.0f; Upper %.0f\n", c->lower, c->upper);
    Serial.printf("Overall gain: %.3e\n", c->overall_gain);
    int i = 0;
    Serial.printf("First order controller:\n");
    while (i < a) {
        Serial.printf(">>> ");
        c->controllers[i++]->show();
    }
    Serial.printf("Single pole controller:\n");
    while (i < b) {
        Serial.printf(">>> ");
        c->controllers[i++]->show();
    }
}

template <int a, int b>
void servo_parser(IIRCascadeController<a, b>* c) {
    int i = 0;
    while (i < a) {
        delete c->controllers[i];
        double z = SerialReader();
        double p = SerialReader();  // no guarantee that p is 0
        c->controllers[i++] = new IIRFirstOrderController(z, p);
    }
    // last controller is always a single pole at -1/2
    while ((i + 1) < b) {
        delete c->controllers[i];
        double p = SerialReader();
        c->controllers[i++] = new IIRSinglePoleController(p);
    }
    // new overall gain
    c->overall_gain = SerialReader();
    // new waveform
    c->reference->set_data_from_serial();
    Serial.printf("Read %d datapoints\n", c->reference->tot);
    // clear everything
    c->reference->clear_reference();
    c->reference->clear_timer();
    // turn on the controller
    c->on = true;
}

template <int a, int b>
void __attribute__((weak))
IIRCascadeController<a, b>::read_from_serial(uint8_t const c) {
    switch (c) {
        case SWEEP: {
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
IIRCascadeController<len_zeroes, len_poles> make_iir_cascade_controller(
    read_func_t reader, write_func_t writer, T const (&zeroes)[len_zeroes],
    U const (&poles)[len_poles], double const overall_gain,
    double const lower = -32768., double const upper = 32767.,
    ReferencePath* reference = &zero_reference) {
    static_assert(!is_int<T>::value,
                  "Initialization error detected. Did you forget to put "
                  "decimal point?\n(e.g. writing -1/3 instead of -1./3");
    static_assert(!is_int<U>::value,
                  "Initialization error detected. Did you forget to put "
                  "decimal point?\n(e.g. writing -1/3 instead of -1./3");
    return IIRCascadeController<len_zeroes, len_poles>(
        reader, writer, zeroes, poles, overall_gain, lower, upper, reference);
}
using PIController = IIRCascadeController<1, 2>;
using PIDController = IIRCascadeController<2, 3>;

#endif