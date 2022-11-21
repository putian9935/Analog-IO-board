#include <Arduino.h>
#include <cstdlib>
#include "intensity_servo_helper.hpp"
#include "serial_reader.hpp"
#include "servo_system.hpp"
#include "tinyfsm.hpp"

struct SerialEvent : tinyfsm::Event {};
struct TurnOnSweep : tinyfsm::Event {};
struct TurnOnServo : tinyfsm::Event {};
struct TurnReadLim : tinyfsm::Event {};

SerialEvent ser;
TurnOnServo servo;
TurnOnSweep sweep;
struct ServoMachine;

struct Servo;
struct Sweep;

using fsm_handle = ServoMachine;

struct ServoMachine : tinyfsm::Fsm<ServoMachine> {
    static PIServoSystem* sweep_sys;
    static uint8_t channel_on;
    virtual void react(SerialEvent const&) {}
    virtual void react(TurnOnServo const&) {}
    virtual void react(TurnOnSweep const&) {}
    virtual void entry(void) {} /* entry actions in some states */
    void exit(void) {}          /* no exit actions */
};

void sweep_parser() {
    uint8_t ch           = SerialReader();
    servoes[ch].sc.lower = SerialReader();
    servoes[ch].sc.upper = SerialReader();
    // update sweep_sys
    ServoMachine::sweep_sys = servoes + ch;
}

void servo_parser() {
    // channel index
    uint8_t ch = SerialReader();
    // new corner
    *(servoes[ch].c->controllers[0]) = IIRFirstOrderController(0, SerialReader());
    // new overall gain
    servoes[ch].c->overall_gain = SerialReader();
    // new waveform
    servoes[ch].c->reference->set_data_from_serial();
    Serial.printf("Read %d datapoints\n", servoes[ch].c->reference->tot);
    ServoMachine::channel_on |= (1 << ch);
}

void channel_parser() {
    uint8_t ch = SerialReader();
    if (ch & (1 << 8)) 
        ServoMachine::channel_on |= (1 << (ch & 3));
    else
        ServoMachine::channel_on &= ~(1 << (ch & 3));
}
struct Idle : ServoMachine {
    void entry() override {
    }
    void react(SerialEvent const&) override {
        uint8_t c = Serial.read();
        switch (c) {
            case 1: {
                sweep_parser();
                auto gp = get_best_power(ServoMachine::sweep_sys);
                Serial.printf("Max power of %d at DAC number %d.\n", gp.pmax, gp.vmax);
                Serial.printf("Min power of %d at DAC number %d.\n", gp.pmin, gp.vmin);
                transit<Sweep>();
                break;
            }
            case 2: {
                servo_parser();
                transit<Servo>();
                break;
            }
            case 3: {
                channel_parser();
                transit<Servo>();
                break;
            }
        }
    }
};

struct Servo : ServoMachine {
    void entry() override {
        servo_loop(ServoMachine::channel_on);
    }
    void react(SerialEvent const&) override { transit<Idle>(); }
    void react(TurnOnServo const&) override { transit<Servo>(); }
};

struct Sweep : ServoMachine {
    void entry() override {
        get_best_power(ServoMachine::sweep_sys);
    }
    void react(SerialEvent const&) override { transit<Idle>(); }
    void react(TurnOnSweep const&) override { transit<Sweep>(); }
};

FSM_INITIAL_STATE(ServoMachine, Idle)

void init_fsm() {
    fsm_handle::start();
}

void state_machine_loop() {
    if (Serial.available())
        fsm_handle::dispatch(ser);
    // if (digitalReadFast(37) == HIGH)
    fsm_handle::dispatch(servo);
    fsm_handle::dispatch(sweep);
}

PIServoSystem* ServoMachine::sweep_sys = servoes;
uint8_t ServoMachine::channel_on       = 0;
