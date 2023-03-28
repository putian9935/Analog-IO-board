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

std::pair<uint16_t, uint16_t> analogio_read_both(uint8_t const ch) {
    if (ch == CH0) {
        return {read_ain0(), read_bin0()};
    }
#ifdef ADC_FOUR_CHANNEL
    else if (ch == CH1) {
        return {read_ain1(), read_bin1()};
    }
#endif
}

decltype(write_both)& analogio_write_both   = write_both;
decltype(write)& analogio_write             = write;
decltype(digitalReadFast)& digitalio_read   = digitalReadFast;
decltype(digitalWriteFast)& digitalio_write = digitalWriteFast;
decltype(pinMode)& digitalio_mode           = pinMode;
