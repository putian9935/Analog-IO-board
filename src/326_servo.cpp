#include "read.hpp"
#include "servo/controllers/iir_controller.hpp"
#include "servo/reference.hpp"
#include "write.hpp"

static ReferencePath refs[8] = {ReferencePath(), ReferencePath(), ReferencePath(), ReferencePath(), ReferencePath(), ReferencePath(), ReferencePath(), ReferencePath()};

static PIController
    servo326{[]() { return (double)read_ain0(); },
             [](double x) { async_write(0, x); },
             {-1 / 5.},
             {0., -1 / 2.},
             -0.03,
             32768.,     // min output DAC number
             34200.,  // max output DAC number
             &refs[0],
             &refs[4],
             };

// overwrite the weak symbols 
extern pController
    pc0 = &servo326;