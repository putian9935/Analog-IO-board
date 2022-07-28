#include <Arduino.h>
#include "read.hpp"
#include "write.hpp"

// a reader function type, i.e. no parameter and return a uint16_t number
typedef uint16_t (*read_func_t)();

/**
 * @brief PID controller class
 *
 * - channel: DAC write channel (0-7);
 * - kp, ki, kd: pid constants;
 * - reference: reference input;
 * - error, integral, last_error: internal variable for calculating output; 
 * - read: a reader function, see below; 
 *
 * Example usage:
 * void loop() {
 *     static PIDController pc(7, 200, 500, 20, 20000, read_bin0); 
 *     pc.update(); 
 * }
 * 
 * @note
 * 1. PID gains are **not** decoupled from time, i.e. if, due to whatever reason, the time of loop() changes, PID gains should also change.
 * 2. Four reader function are implemented in reader.cpp, namely, read_ain0, read_ain1, read_bin0, read_bin1.
 *   You can implement your own reader function and use it, e.g. 
 * 
 * uint16_t my_reader() {
 *     return (ain0 + bin0) >> 1; // connect same source to multiple channel for better noise reduction 
 * }
 */

struct PIDController
{
    uint8_t channel;
    double kp, ki, kd;
    double reference;
    double last_error, error, integral;
    read_func_t read;
    PIDController() = delete; 
    PIDController(uint8_t ch)
    {
        channel    = ch;
        last_error = 0;
        integral   = 0;
        kp = ki = kd = 0;
        reference    = 0;
        read         = []() -> uint16_t { return 0; };
    }
    PIDController(uint8_t ch, double kp, double ki, double kd, double ref, read_func_t reader)
    {
        channel    = ch;
        last_error = 0;
        integral   = 0;
        this->kp   = kp;
        this->ki   = ki;
        this->kd   = kd;
        reference  = ref;
        read       = reader;
    }
    void update()
    {
        error = (read() - reference) * 1. / 65535;

        int32_t dac_num = -(kp * error + ki * integral + kd * (error - last_error));
 
        if (dac_num > 32000)
            dac_num = 32000; // ensure single side, and anti-windup
        else if (dac_num < 0)
            dac_num = 0; // ensure single side, and anti-windup
        else
            integral += error;
        write(channel, (uint16_t)dac_num);
        last_error = error;
    }
};
