/**
 * @file serial_reader.hpp
 * @brief Serial reader helper class. 
 * @date 2022-11-28
 * 
 * 
 */
#ifndef SERIAL_READER
#define SERIAL_READER 

#include <Arduino.h> 
#include <cstring>

// We need a function that has overload resolution by return value;
// this can be done by abusing the cast operation  
struct SerialReader {
    static char buf[16]; // Type "long double" is of size 16
    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    operator T() {
        T ret;
        Serial.readBytes(buf, sizeof(T));
        std::memcpy(&ret, buf, sizeof(T));
        return ret;
    }
};

#endif 