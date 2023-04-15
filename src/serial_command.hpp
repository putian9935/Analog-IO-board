#ifndef SERIAL_COMMAND_HPP
#define SERIAL_COMMAND_HPP

#include <stdint.h> 

enum Command : uint8_t {
    SWEEP = 1,
    SERVO,
    CHANNEL,
    HSP, 
    SHOW
};


#endif 