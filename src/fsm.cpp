#include <Arduino.h>
#include <cstdlib>
#include "intensity_servo_helper.hpp"
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
    virtual void react(SerialEvent const&) {}
    virtual void react(TurnOnServo const&) {}
    virtual void react(TurnOnSweep const&) {}
    virtual void entry(void) {} /* entry actions in some states */
    void exit(void) {}          /* no exit actions */
};

struct Idle : ServoMachine {
    void entry() override {
        Serial.println("Current state Idle");
    }
    void react(SerialEvent const&) override {
        uint8_t c = Serial.read();
        Serial.println(c);
        switch (c) {
            case 0:
                transit<Sweep>();
                break;
            case 2:
                transit<Servo>();
                break;
        }
    }
};

struct Servo : ServoMachine {
    void entry() override {
        step_response();
    }
    void react(SerialEvent const&) override { transit<Idle>(); }
    void react(TurnOnServo const&) override { transit<Servo>(); }
};

struct Sweep : ServoMachine {
    void entry() override {
        auto gp = get_best_power(&ic_410_master);
        Serial.printf("Max power of %d at DAC number %d.\n", gp.pmax, gp.vmax);
        Serial.printf("Min power of %d at DAC number %d.\n", gp.pmin, gp.vmin);
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
    if (digitalReadFast(37) == HIGH)
        fsm_handle::dispatch(servo);
    fsm_handle::dispatch(sweep);
}