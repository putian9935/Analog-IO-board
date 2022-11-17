#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"
#include "controller.cpp"

void setup()
{
    while (!Serial)
        ;
    Serial.begin(115200);
    init_chips();
}

auto reference_ic_410_master=33400, reference_ic_410_slave=33000;

void loop()
{

    static auto ic_410_master = make_iir_cascade_controller(
        [](){return (double)read_ain0() - reference_ic_410_master;},
        [](double x){write(4, x);},
        {-1/3.},
        {0., -1/2.},
        -0.015   ,
        0.,
        950.
        );

    static auto ic_410_slave = make_iir_cascade_controller(
        [](){return (double)read_ain1() - reference_ic_410_slave;},
        [](double x){write(5, x);},
        {-1/3.},
        {0., -1/2.},
        -0.015   ,
        0.,
        820.
        );

    reference_ic_410_master = 33400;
    reference_ic_410_slave = 33000;
    for(int i = 0;i < 5000; ++i)
    {
        ic_410_master.update();
        ic_410_slave.update();
    }

    reference_ic_410_master = 33700;
    for(int i = 0;i < 5000; ++i)
    {
        ic_410_master.update();
        ic_410_slave.update();
    }

    reference_ic_410_master = 33400;
    reference_ic_410_slave = 33450;
    for(int i = 0;i < 5000; ++i)
    {
        ic_410_master.update();
        ic_410_slave.update();
    }

    reference_ic_410_master = 33700;
    for(int i = 0;i < 5000; ++i)
    {
        ic_410_master.update();
        ic_410_slave.update();
    }

}
