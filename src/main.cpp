#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"
#include "simulator.cpp"


void setup()
{
    while (!Serial)
        ;
    Serial.begin(115200);
}

void loop()
{
    auto ic = make_iir_cascade_simulator(
    {-1./5},
    {0., -1./3},
    -250. * 65536 / 6.,
    [](double const x) {return 4.1e-4 * x * 6 /65536 ;}
    ); 

    if(Serial.available())
    {
        Serial.read();
        simulate(ic);
    }
}
