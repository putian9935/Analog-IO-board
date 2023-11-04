
#include "servo/controller.hpp"
#include "servo/intensity_servo_helper.hpp"

struct VanillaPIController : public Controller {
    float kp, ki, integral;
    float last_out;

    VanillaPIController(read_func_t reader, write_func_t writer,
                        double const kp, double const ki,
                        double const lower,
                        double const upper, ReferencePath* reference,
                        ReferencePath* reference_hsp)
        : Controller(reader, writer, reference, reference_hsp, lower, upper),
          kp(kp),
          ki(ki),
          last_out(0.) {
    }

    
    void update() override {
        float error =
            (reader() - reference->get_reference());  // start with an error
        integral += error; 
        float new_out = kp * error + ki * integral + 32767.;
        if (new_out > upper ) {
            new_out = upper; 
            integral -= error; 
        }
        else if (new_out < lower ) {
            new_out = lower; 
            integral -= error; 
        }
        writer((uint16_t)new_out);
    }

    void clear() {
        // reset the integral 
        integral = 0; 
    }

    void read_from_serial(uint8_t const) override;
};


void sweep_parser(VanillaPIController* c) {
    c->lower = (uint16_t) SerialReader();
    c->upper = (uint16_t) SerialReader();
    uint16_t step = SerialReader();
    auto gp = get_best_power(c, step);
    Serial.printf("Max power of %d at DAC number %d.\n", gp.pmax, gp.vmax);
    Serial.printf("Min power of %d at DAC number %d.\n", gp.pmin, gp.vmin);
}

void show_parser(VanillaPIController* c) {
    Serial.printf("Lower: %.0f; Upper %.0f\n", c->lower, c->upper);
    Serial.printf("kp %f, ki %f\n", c->kp, c->ki);
}

void servo_parser(VanillaPIController* c) {
    c->kp = (double) SerialReader(); 
    c->ki =(double) SerialReader(); 
    Serial.printf("kp %f, ki %f\n", c->kp, c->ki);
}

void ref_parser(VanillaPIController* c) {
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

void hsp_parser(VanillaPIController* c) {
    // new waveform
    c->reference_hsp->set_data_from_serial();
    Serial.printf("Read %d datapoints\n", c->reference_hsp->tot);
    // clear everything
    c->reference_hsp->clear_reference();
    c->reference_hsp->clear_timer();
    c->clear();
    c->on = true;
}

void VanillaPIController::read_from_serial(uint8_t const c) {
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

