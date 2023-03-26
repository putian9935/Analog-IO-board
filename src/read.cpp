/**
 * @file read.cpp
 * @brief Interface for getting ADC readings
 * @date 2022-11-28
 *
 */
#include "read.hpp"
#include "bit_mangler.h"

static std::pair<uint16_t, uint16_t> decode(uint32_t data) {
    return {decode_32_16(data >> 1), decode_32_16(data)};
}

namespace {
    uint32_t ch0(0), ch1(0);
    bool decoded0(false), decoded1(false);
    std::pair<uint16_t, uint16_t> decoded_ch0, decoded_ch1;
    uint16_t const
        &ain0 = decoded_ch0.first,
        &ain1 = decoded_ch1.first,
        &bin0 = decoded_ch0.second,
        &bin1 = decoded_ch1.second;
    uint8_t life;
    void read() {
        static IMXRT_LPSPI_t* spi_regs = &IMXRT_LPSPI4_S;
        if (!(life--)) {
            spi_regs->TCR |= LPSPI_TCR_TXMSK;
            while (spi_regs->RSR & LPSPI_RSR_RXEMPTY)
                ;
            spi_regs->TCR |= LPSPI_TCR_TXMSK;
            ch0 = spi_regs->RDR;
            while (spi_regs->RSR & LPSPI_RSR_RXEMPTY)
                ;
            ch1 = spi_regs->RDR;
            decoded0 = decoded1 = false;
            life                = 3;
        }
    }
};

uint16_t read_ain0() {
    read();
    if (!decoded0) {
        decoded_ch0 = decode(ch0);
        decoded0    = true;
    }
    return ain0;
}

uint16_t read_ain1() {
    read();
    if (!decoded1) {
        decoded_ch1 = decode(ch1);
        decoded1    = true;
    }
    return ain1;
}

uint16_t read_bin0() {
    read();
    if (!decoded0) {
        decoded_ch0 = decode(ch0);
        decoded0    = true;
    }
    return bin0;
}

uint16_t read_bin1() {
    read();
    if (!decoded1) {
        decoded_ch1 = decode(ch1);
        decoded1    = true;
    }
    return bin1;
}