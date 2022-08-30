#include "controller.cpp"
#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"

double reference0 = 32000;
bool ramping      = false;

elapsedMicros t = 0;
void start_ramping_isr()
{
    t          = 0;
    ramping    = true;
    reference0 = 31790;
}

void setup()
{
    while (!Serial)
        ;
    Serial.begin(115200);
    init_chips();

    attachInterrupt(15, start_ramping_isr, FALLING);
}

void loop()
{
    static auto ic2 = make_iir_controller(
        []() { return (double)(read_ain1()) - 32768; },
        [](double x) { write(6, x); },
        {0.},
        {0.});
    static auto ic = make_iir_cascade_controller(
        []() { return read_ain0() - reference0; },
        [](double x) { write(7, x); },
        {-1. / 500, -1. / 3},
        {0., 0., -1. / 1},
        .28,
        0., 800.);

    ic.update();
    ic2.update();
    ic2.update();
    ic2.update();

    if (ramping) {
        reference0 = 31790. + 210. / 500. * t;
        if (reference0 > 32000) {
            ramping    = false;
            reference0 = 31790;
        }
    }

    // Serial.printf("%d %d %d %d\n", ain0, ain1, bin0, bin1);
}
