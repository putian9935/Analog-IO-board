#include "servo_system.hpp"

#include "read.hpp"
#include "write.hpp"

ReferencePath refs[4] = {};
PIController controllers[] = {
    make_iir_cascade_controller(
    []() { return (double)read_ain0(); },
    [](double x) { write(4, x); },
    {-1 / 3.},
    {0., -1 / 2.},
    -0.015,
    0.,
    950.,
    &refs[0]),
    make_iir_cascade_controller(
    []() { return (double)read_ain1(); },
    [](double x) { write(5, x); },
    {-1 / 3.},
    {0., -1 / 2.},
    -0.015,
    0.,
    950.,
    &refs[1]),
    make_iir_cascade_controller(
    []() { return (double)read_bin0(); },
    [](double x) { write(6, x); },
    {-1 / 3.},
    {0., -1 / 2.},
    -0.015,
    0.,
    950.,
    &refs[2]),
    make_iir_cascade_controller(
    []() { return (double)read_bin1(); },
    [](double x) { write(7, x); },
    {-1 / 3.},
    {0., -1 / 2.},
    -0.015,
    0.,
    950.,
    &refs[3])
};

PIServoSystem servoes[] = {
    PIServoSystem(controllers),
    PIServoSystem(controllers+1),
    PIServoSystem(controllers+2),
    PIServoSystem(controllers+3)
};
