#include "read.hpp"
#include "servo/controllers/iir_controller.hpp"
#include "servo/reference.hpp"
#include "write.hpp"

static ReferencePath refs[8] = {ReferencePath(), ReferencePath(), ReferencePath(), ReferencePath(), ReferencePath(), ReferencePath(), ReferencePath(), ReferencePath()};

static PIController
    servo707{[]() { return (double)read_ain0(); },
             [](double x) { write(4, x); },
             {-1 / 3.},
             {0., -1 / 2.},
             -0.025,
             0.,     // min output DAC number
             1500.,  // max output DAC number
             &refs[0],
             &refs[4],
             },
    servo679{[]() { return (double)read_ain1(); },
             [](double x) { write(5, x); },
             {-1 / 4.},
             {0., -1 / 2.},
             -0.03,
             0.,
             1500.,
             &refs[1],
             &refs[5]
             },
    servoCoil{[]() { return (double)read_bin0(); },
              [](double x) { write(6, x); },
              {-1 / 5.},
              {0., -1 / 2.},
              0.004,
              -3000.,
              26000.,
              &refs[2],
              &refs[6]
              },
    servoMOT{[]() { return (double)read_bin1(); },
             [](double x) { write(7, x); },
             {-1 / 3.5},
             {0., -1 / 2.},
             -0.07,
             0.,
             1500.,
             &refs[3],
             &refs[7],
             };

// overwrite the weak symbols 
extern pController
    pc0 = &servo707,
    pc1 = &servo679,
    pc2 = &servoCoil,
    pc3 = &servoMOT;
