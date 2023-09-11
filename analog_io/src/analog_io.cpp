#include "analog_io.h"

uint16_t analogio_read(uint8_t const ch) {
    switch (ch) {
        case AIN0:
            return read_ain0();
        case BIN0:
            return read_bin0();
#ifdef ADC_FOUR_CHANNEL
        case AIN1:
            return read_ain1();
        case BIN1:
            return read_bin1();
#endif
    }
}


decltype(async_write)& analogio_write             = async_write;
decltype(digitalReadFast)& digitalio_read   = digitalReadFast;
decltype(digitalWriteFast)& digitalio_write = digitalWriteFast;
decltype(pinMode)& digitalio_mode           = pinMode;
