#include "servo/servo_system.hpp"

typedef Controller* const pController;
extern pController pc0, pc1, pc2, pc3;
Controller* const servoes[] = {
    pc0,
    pc1,
    pc2,
    pc3,
};
