#include "init_chips.hpp"
extern void init_fsm();
extern void state_machine_loop();

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
