#ifndef SERIAL_READER
#define SERIAL_READER 

#include <Arduino.h> 

struct SerialReader {
    static char buf[8];
    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
    operator T() {
        T ret;
        Serial.readBytes(buf, sizeof(T));
        std::memcpy(&ret, buf, sizeof(T));
        return ret;
    }
};

#endif 