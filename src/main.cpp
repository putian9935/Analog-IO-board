#include "analog_io.h"
#include "pin_assignment.h"
#include "trigger.h"
#include "fsm.hpp"

void setup() {
    init_chips();
    while (!Serial)
        ;
    Serial.begin(115200);
    init_trigger();
    init_fsm();
    delay(200);
    pinMode(40, OUTPUT);
}

void loop() {
    fsm_loop();
}
