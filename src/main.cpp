#include "controller.cpp"
#include "init_chips.hpp"
#include "read.hpp"
#include "reference.hpp"
#include "write.hpp"

void setup()
{
    while (!Serial)
        ;
    Serial.begin(115200);
    init_chips();
}

void loop()
{
    static double const t_val[] = {0, 100, 200, 1000, 1100, 1200, 1600, 2200, 2300, 5000};
    static double const v_val[] = {33630, 33630, 33100, 33100, 33350, 33300, 33430, 33430, 33100, 33100};
    // static Reference ref(10, t_val, v_val, 33100);
    static auto ref = make_reference_from_array(t_val, v_val);

    static auto ic_410_master = make_iir_cascade_controller(
        []() { return read_ain0() - ref.get_reference(); },
        [](double x) { write(4, x); },
        {-1 / 3.},
        {0., -1 / 2.},
        -0.015,
        0.,
        950.);

    Reference::clear_timer();
    ref.clear_reference();
    while (!ref.is_terminated())
        ic_410_master.update();
}
