// files other than ".ino" has to include this header
// otherwise it would be an link error 
#include <Arduino.h>

template<typename T>
void printBinary(T value)
{
    for ( size_t mask = 1 << ((sizeof(value) * 8) - 1); mask; mask >>= 1 )
    {
        Serial.print(value & mask ? "1" : "0");
    }
    Serial.print('\n');
}
