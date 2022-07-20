#include "clockspeed.h"
#include "init_chips.hpp"
#include "read.hpp"
#include "write.hpp"

void setup()
{
    // This will set a higher IPG root clock
    CCM_CBCDR = (CCM_CBCDR & ~CCM_CBCDR_IPG_PODF_MASK) | CCM_CBCDR_IPG_PODF(1);
    // while (!Serial)
    ;
    Serial.begin(115200);
    set_arm_clock_cpp(800000000);

    init_chips();
}

#define MAXN 50

#define SAMPLE 64
#define SAMPLE_LOG 6

uint16_t read()
{
    uint32_t ret = 0;
    for (int j = 0; j < SAMPLE; ++j)
        ret += ain0;
    return (ret >> SAMPLE_LOG);
}

struct PIDController
{
    uint8_t channel;
    double last_error, error, integral;
    double kp, ki, kd;
    double reference;
    PIDController(uint8_t ch)
    {
        channel    = ch;
        last_error = 0;
        integral   = 0;
        kp = ki = kd = 0;
        reference    = 0;
    }
    PIDController(uint8_t ch, double kp, double ki, double kd, double ref)
    {
        channel    = ch;
        last_error = 0;
        integral   = 0;
        this->kp   = kp;
        this->ki   = ki;
        this->kd   = kd;
        reference  = ref;
    }
    void update()
    {
        error = (read() - reference) * 1. / 65535;

        int32_t dac_num = -(kp * error + ki * integral + kd * (error - last_error));
        if (dac_num > 5000)
            dac_num = 5000; // ensure single side
        else if (dac_num < 0)
            dac_num = 0;
        else 
            integral += error;
        write(channel, (uint16_t)dac_num);
        last_error = error;
    }
};

void loop()
{
    // static PIDController pidc(0, 450, 450, 250, 55000); // 256 samples
    static PIDController pidc(0, 450, 150, 50, 55000); // 64 samples
    pidc.reference = 40000;
    for (int i = 0; i < 300; ++i)
    {
        pidc.update();
    }
    pidc.reference = 0;
    for (int i = 0; i < 300; ++i)
    {
        pidc.update();
    }
}
