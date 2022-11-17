#include <Arduino.h>
#include "write.hpp"
#include "read.hpp"
#include "controller.cpp"

void get_adc_reading_limits(){
    write(4,1015);
    delay(1);
    Serial.print(read_ain0());
    Serial.print(' ');
    write(4,0);
    delay(1);

    Serial.print(read_ain0());
    Serial.print(' ');
    write(5,917);
    delay(1);

    Serial.print(read_ain1());
    Serial.print(' ');
    write(5,0);
    delay(1);

    Serial.print(read_ain1());
    Serial.print('\n');
    delay(1000);

}

void get_best_power() {
    for(int i = 0; i < 1500; ++i) {
        write(5, i);
        write(4, i);
        delay(1);
    }
}

auto reference_ic_410_master=33050., reference_ic_410_slave=33000.;

void step_response() {
    static auto ic_410_master = make_iir_cascade_controller(
        [](){return  read_ain0() -  reference_ic_410_master;},
        [](double x){write(4, x);},
        {-1/3.},
        {0., -1/2.},
        -0.015,
        0.,
        950.
        );
    static auto ic_410_slave = make_iir_cascade_controller(
        [](){return read_ain1() - reference_ic_410_slave;},
        [](double x){write(5, x);},
        {-1/3.},
        {0., -1/2.},
        -0.015,
        0.,
        820.
        );

    reference_ic_410_master = 33050;
    reference_ic_410_slave = 32900;
    for(int i = 0;i < 5000; ++i)
    {
        reference_ic_410_master += 0.116;
        ic_410_master.update();
        ic_410_slave.update();
    }

    reference_ic_410_master = 33630;
    for(int i = 0;i < 5000; ++i)
    {
        reference_ic_410_master -= 0.116;
        ic_410_master.update();
        ic_410_slave.update();
    }

    reference_ic_410_master = 33050;
    reference_ic_410_slave = 33450;
    for(int i = 0;i < 5000; ++i)
    {
        ic_410_master.update();
        ic_410_slave.update();
    }

    reference_ic_410_master = 33630;
    for(int i = 0;i < 5000; ++i)
    {
        ic_410_master.update();
        ic_410_slave.update();
    }
}