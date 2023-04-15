#include "pi_controller.hpp"
#include "serial_reader.hpp"
#include "intensity_servo_helper.hpp"


void sweep_parser(PIController* c) {
    c->lower = (uint16_t)SerialReader();   
    c->upper = (uint16_t)SerialReader();
    auto gp = get_best_power(c);  
    Serial.printf("Max power of %d at DAC number %d.\n", gp.pmax, gp.vmax);
    Serial.printf("Min power of %d at DAC number %d.\n", gp.pmin, gp.vmin);
}

void servo_parser(PIController* c) {
    // new corner
    delete c->controllers[0];
    c->controllers[0] = new IIRFirstOrderController(SerialReader(), 0);
    // new overall gain
    c->overall_gain = SerialReader();
    // new waveform
    c->reference->set_data_from_serial();
    Serial.printf("Read %d datapoints\n", c->reference->tot);
    // clear everything
    c->reference -> clear_reference();
    c->reference -> clear_timer();
    // turn on the controller 
    c->on = true; 
}

void show_parser(PIController* c) {
    Serial.printf("Lower: %.0f; Upper %.0f\n", c->lower, c->upper); 
    Serial.printf("Overall gain: %.3e\n", c->overall_gain); 
    Serial.printf("First order controller:\n");
    c->controllers[0]->show();
    Serial.printf("Single pole controller:\n");
    c->controllers[1]->show();
}
