#include <Arduino.h>
#include "write.hpp"
#include "read.hpp"

void get_adc_reading_limits(){
    Serial.println(read_ain1());
    delay(1000);
}

void get_best_power() {
    for(int i = 0; i < 1500; ++i) {
        write(5, i);
        delay(1);
    }
}
