#include "pi_controller.hpp"
#include "serial_reader.hpp"
#include "servo/intensity_servo_helper.hpp"


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

