#include <DMAChannel.h>
#include "analog_io.h"
#include "fsm2.h"
#include "serial_reader.hpp"
#include "servo/controllers/iir_controller.hpp"
#include "servo/intensity_servo_helper.hpp"
#include "servo/servo_system.hpp"

enum class States {
    Idle,
    Servo,
    Sweep
};

enum class Triggers { Heartbeat,
                      SerialEvent,
                      TurnOnSweep,
                      TurnOnServo };

using F = FSM::Fsm<States, States::Idle, Triggers>;
F fsm;

extern DMAChannel rx, tx;
static Triggers next_trig = Triggers::Heartbeat;

static void servo_loop() {
    digitalWriteFast(40, HIGH);
    auto gain  = 1.2;
    double x   = read_ain0();
    double err = (x - 32768.);
    async_write(0, ((int16_t)(-(double)(err)*gain) + 32768));
    digitalWriteFast(40, LOW);

    for (int ch = 1; ch < 4; ++ch) {
        if (servoes[ch]->on)
            servoes[ch]->update();
    }
}
static Controller *sweep_c;

static void channel_parser(uint8_t ch) {
    if (ch & (1 << 7))
        servoes[ch & 3]->on = true;
    else
        servoes[ch & 3]->on = false;
    servoes[ch & 3]->reference->clear_reference();
    servoes[ch & 3]->reference->clear_timer();
    for (int i = 0; i < 4; ++i)
        Serial.printf("CH%d: %s; ", i, (servoes[i]->on) ? "*" : "o");
    Serial.printf("\n");
}

void sweep_loop() {
    get_best_power(sweep_c);
}

void handle_serial() {
    rx.enable();
    tx.enable();
    uint8_t c  = SerialReader();
    uint8_t ch = SerialReader();

    next_trig = Triggers::Heartbeat;
    switch (c) {
        case CHANNEL: {
            channel_parser(ch);
            next_trig = Triggers::TurnOnServo;
            break;
        }
        default: {
            servoes[ch]->read_from_serial(c);
            // delay for continuous update
            delay(4);
            if (Serial.available())
                next_trig = Triggers::SerialEvent;
            else {
                if (c == SWEEP) {
                    sweep_c   = servoes[ch];
                    next_trig = Triggers::TurnOnSweep;
                } else if (c == SERVO || c == REF || c == HSP)
                    next_trig = Triggers::TurnOnServo;
            }
            break;
        }
    }
    if (!(next_trig == Triggers::TurnOnSweep || next_trig == Triggers::TurnOnServo)) {
        rx.disable();
        tx.disable();
    }
}

void reset_next_trig() {
    next_trig = Triggers::Heartbeat;
}

void init_fsm() {
    fsm.add_transitions({
        {States::Idle, States::Idle, Triggers::Heartbeat, nullptr, reset_next_trig},
        {States::Idle, States::Idle, Triggers::SerialEvent, nullptr, handle_serial},
        {States::Idle, States::Servo, Triggers::TurnOnServo, nullptr, reset_next_trig},
        {States::Idle, States::Sweep, Triggers::TurnOnSweep, nullptr, reset_next_trig},
        {States::Servo, States::Servo, Triggers::Heartbeat, nullptr, servo_loop},
        {States::Sweep, States::Sweep, Triggers::Heartbeat, nullptr, sweep_loop},
        {States::Servo, States::Idle, Triggers::SerialEvent, nullptr, handle_serial},
        {States::Sweep, States::Idle, Triggers::SerialEvent, nullptr, handle_serial},
    });
}

uint8_t do_not_check = 0;

void fsm_loop() {
    if (!(do_not_check++) && Serial.available()) {
        next_trig = Triggers::SerialEvent;
    }
    fsm.execute(next_trig);
}
