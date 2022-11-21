#include "servo_system.hpp"

#include "read.hpp"
#include "write.hpp"


ReferencePath ref_410_master;
auto ic_410_master = make_iir_cascade_controller(
    []() { return (double)read_ain0(); },
    [](double x) { write(4, x); },
    {-1 / 3.},
    {0., -1 / 2.},
    -0.015,
    0.,
    950.,
    &ref_410_master);

PIServoSystem master_410_servo(&ic_410_master);
