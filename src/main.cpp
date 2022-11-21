#include "init_chips.hpp"
#include "fsm.hpp"

void setup() {
    while (!Serial)
        ;
    Serial.begin(115200);
    init_chips();
    init_fsm();
    pinMode(37, INPUT);
}

void loop() {
    state_machine_loop();
}
