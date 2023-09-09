#include "analog_io.h"
#include "pin_assignment.h"
#include "trigger.h"

extern void init_fsm();
void setup() {
    while (!Serial)
        ;
    Serial.begin(115200);

    init_chips();
    init_trigger();
    init_fsm();
    delay(200);
    pinMode(40, OUTPUT);
}

// extern void test5(double);

void test5(double gain) {
    // one/two channel 1.3; three channel 1.2

    while (1) {
        int16_t x   = read_ain0();
        int16_t err = (x - 32768);
        write(0, ((int16_t)(-(double)(err)*gain) + 32768));

        read_ain0();
        write(1, x);

        // read_ain0();
        // write(2, x);

        // read_ain0();
        // write(2, x);
    }
}

extern void fsm_loop();

void loop() {
    fsm_loop();
    // test5(1.3);
}
