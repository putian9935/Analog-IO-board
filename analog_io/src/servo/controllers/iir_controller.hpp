#ifndef IIR_CONTROLLER
#define IIR_CONTROLLER

#include "servo/controller.hpp"
#include "servo/intensity_servo_helper.hpp"

// first order IIR controller base class
struct IIRBaseController {
    float le, lo;  // last error and last output
    IIRBaseController() : le{}, lo{} {}
    void clear() {
        le = 0;
        lo = 0;
    }
    virtual float transfer(float const err) = 0;
    virtual void show() = 0;
    virtual ~IIRBaseController(){};
};

// first order IIR controller with zero and pole
struct IIRFirstOrderController : public IIRBaseController {
    float a, b, c;
    double z, p;
    IIRFirstOrderController(float const z, float const p)
        : a((1 + M_PI * p) / (1 - M_PI * p)),
          b((1 + M_PI * z) / (1 - M_PI * z)),
          c((1 - M_PI * z) / (1 - M_PI * p)),
          z(z),
          p(p) {}
    IIRFirstOrderController(IIRFirstOrderController const& old) = default;
    IIRFirstOrderController(IIRFirstOrderController&& old) = default;

    IIRFirstOrderController& operator=(const IIRFirstOrderController& old) {
        z = old.z;
        p = old.p;
        a = old.a;
        b = old.b;
        c = old.c;
        return *this;
    }
    IIRFirstOrderController() : z(0), p(0) {}
    float transfer(float const e) override {
        float ret = (a * lo - b * le + e);
        ret = max(min(ret, 32767.), -32768.);
        lo = ret;
        le = e;
        return ret;
    }
    void show() override { Serial.printf("Zero: %.3f; Pole: %.3f\n", z, p); }
};

// first order IIR controller with only pole
struct IIRSinglePoleController : public IIRBaseController {
    float a, b;
    double p;
    IIRSinglePoleController(double const p)
        : a(M_PI / (1 - M_PI * p)), b((1 + M_PI * p) / (1 - M_PI * p)), p(p) {}
    IIRSinglePoleController() : p(0) {}

    IIRSinglePoleController(IIRSinglePoleController const&) = default;
    IIRSinglePoleController(IIRSinglePoleController&&) = default;
    IIRSinglePoleController& operator=(IIRSinglePoleController const& rhs) {
        p = rhs.p;
        a = rhs.a;
        b = rhs.b;
        return *this;
    }
    float transfer(float const e) override {
        float ret = (b * lo + (le + e));
        ret = max(min(ret, 32767.), -32768.);
        lo = ret;
        le = e;
        return ret;
    }
    void show() override { Serial.printf("Pole: %.3f\n", p); }
};

// IIR controller in cascade form
template <int len_zeroes, int len_poles>
struct IIRCascadeController : public Controller {
    // static_assert((len_zeroes == 0 && len_poles == 0) ||
    //                   (len_zeroes < len_poles),
    //               "There should be more poles than zeroes. ");

    float overall_gain;
    float last_out;
    float real_g;
    IIRFirstOrderController focontrollers[len_zeroes];
    IIRSinglePoleController spcontrollers[len_poles - len_zeroes];
    IIRCascadeController(read_func_t reader, write_func_t writer,
                         double const (&zeroes)[len_zeroes],
                         double const (&poles)[len_poles],
                         double const overall_gain, double const lower,
                         double const upper, ReferencePath* reference,
                         ReferencePath* reference_hsp)
        : Controller(reader, writer, reference, reference_hsp, lower, upper),
          overall_gain(overall_gain),
          last_out(0.) {
        int i = 0, j = 0;
        real_g = overall_gain;
        for (; i < len_zeroes; ++i) {
            focontrollers[i] = IIRFirstOrderController(zeroes[i], poles[i]);
            real_g *= focontrollers[i].a;
        }
        for (; i < len_poles; ++i, ++j) {
            spcontrollers[j] = IIRSinglePoleController(poles[i]);
            real_g *= spcontrollers[j].a;
        }
    }

    void update() override {
        /**
         * @brief benchmark
         *
         * zero channel: 1590k -- 629 ns
         *
         * one channel : 458 ns (total count, no opt)
         *
         * all: 920k  -- 1087 ns
         * no reference: 930k -- 1075 ns -> reference: 12 ns
         * no transfer: 1250k -- 800 ns -> transfer: 287 ns
         * no out: 980k  -- 1020 ns -> out: 67 ns
         *
         *
         * all (no polymorphism): 980k -> 1020 ns
         * all (minimal fp): 1160k -> 862 ns
         *
         * transfer (minimal fp): 52 ns
         *
         * all (abs -> fabs): 1190k -> 842 ns
         * no reader: 1270k -> 787 ns -> reader() 55 ns
         *
         * all (async write): 1960k -> 510 ns
         * two channels : 920k -> 1086 ns
         *
         * three channels : 781k -> 1280 ns
         *
         * decode: 13 ns
         *
         *
         * async write:
         * (1) 2.06  485ns
         * (2) 1.39  719ns
         * (3) 1.03  970ns
         */
        float new_out =
            (reader() - reference->get_reference());  // start with an error
        // Serial.printf("%lf\n", reference->get_reference());
        int i = 0, j = 0;
        for (; i < len_zeroes; ++i) {
            new_out = focontrollers[i].transfer(new_out);
        }
        for (; i < len_poles; ++i, ++j) {
            new_out = spcontrollers[j].transfer(new_out);
        }
        new_out = max(min(real_g * new_out, upper), lower);
        if (fabs(new_out - last_out) > 1.f)  // lazy update
        {
            last_out = new_out;
            writer((uint16_t)last_out);
        }
    }

    void clear() {
        int i = 0, j = 0;
        for (; i < len_zeroes; ++i) focontrollers[i].clear();
        for (; i < len_poles; ++i, ++j) spcontrollers[j].clear();
    }

    void read_from_serial(uint8_t const) override;
};

template <int a, int b>
void sweep_parser(IIRCascadeController<a, b>* c) {
    c->lower = (uint16_t)SerialReader();
    c->upper = (uint16_t)SerialReader();
    uint16_t step = SerialReader();
    auto gp = get_best_power(c, step);
    Serial.printf("Max power of %d at DAC number %d.\n", gp.pmax, gp.vmax);
    Serial.printf("Min power of %d at DAC number %d.\n", gp.pmin, gp.vmin);
}

template <int a, int b>
void show_parser(IIRCascadeController<a, b>* c) {
    Serial.printf("Lower: %.0f; Upper %.0f\n", c->lower, c->upper);
    Serial.printf("Overall gain: %.3e\n", c->overall_gain);
    Serial.printf("First order controller:\n");
    int i = 0, j = 0;
    for (; i < a; ++i) {
        Serial.printf(">>> ");
        c->focontrollers[i].show();
    }
    Serial.printf("Single pole controller:\n");
    for (; i < b; ++i, ++j) {
        Serial.printf(">>> ");
        c->spcontrollers[j].show();
    }
}

template <int a, int b>
void servo_parser(IIRCascadeController<a, b>* c) {
    int i = 0, j = 0;
    c->real_g = 1;
    for (; i < a; ++i) {
        double z = SerialReader();
        double p = SerialReader();  // no guarantee that p is 0
        c->focontrollers[i] = IIRFirstOrderController(z, p);
        c->real_g *= c->focontrollers[i].a;
    }
    // last controller is always a single pole at -1/2
    for (; i < (b - 1); ++i, ++j) {
        double p = SerialReader();
        c->spcontrollers[j] = IIRSinglePoleController(p);
        c->real_g *= c->spcontrollers[j].a;
    }
    c->real_g *= c->spcontrollers[j].a;
    // new overall gain
    c->overall_gain = (double)SerialReader();
    Serial.printf("overall_gain %f\n", c->overall_gain);
    c->real_g *= c->overall_gain;
}

template <int a, int b>
void ref_parser(IIRCascadeController<a, b>* c) {
    // new waveform
    c->reference->set_data_from_serial();
    Serial.printf("Read %d datapoints\n", c->reference->tot);
    // clear everything
    c->reference->clear_reference();
    c->reference->clear_timer();
    c->clear();
    // turn on the controller
    c->on = true;
}

template <int a, int b>
void hsp_parser(IIRCascadeController<a, b>* c) {
    // new waveform
    c->reference_hsp->set_data_from_serial();
    Serial.printf("Read %d datapoints\n", c->reference_hsp->tot);
    // clear everything
    c->reference_hsp->clear_reference();
    c->reference_hsp->clear_timer();
    c->clear();
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
        }
        case REF: {
            ref_parser(this);
            break;
        }
        case HSP: {
            hsp_parser(this);
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
using PI2DController = IIRCascadeController<3, 4>;
using ConstantController = IIRCascadeController<0, 0>;

#endif