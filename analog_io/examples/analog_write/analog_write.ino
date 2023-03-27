/**
 * @file analog_write.ino
 * @author Li Putian
 * @brief Basic use of analog write. 
 * @date 2023-03-27
 * 
 * 
 */
#include "analog_io.h" 

void setup() {
    while(!Serial); 
    Serial.begin(9600); 

    // initialize the chips 
    init_chips(); 
}

void loop() {
    for(int i = -32768; i < 32768; ++i) {
        // write DAC number i to channel 0
        analogio_write(0, i); 
        // update channel 0 and channel 4 simultaneously 
        // analogio_write_both(0, i, 4, i); 
    }
}
