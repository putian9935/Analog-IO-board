#include "controller.cpp"
#include "init_chips.hpp"
#include "read.hpp"
#include "reference.hpp"
#include "tinyfsm.hpp"
#include "write.hpp"
#include <cstdlib>

extern void init_fsm();
extern void state_machine_loop();

void setup()
{
    while (!Serial)
        ;
    Serial.begin(115200);
    init_chips();
    init_fsm();
    Serial.println("Arduino is ready.");
    pinMode(37, INPUT);
}

void loop()
{
    state_machine_loop();
}
