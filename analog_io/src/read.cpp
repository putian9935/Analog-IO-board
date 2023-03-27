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
    uint8_t life;

    uint32_t ch0(0);
    bool decoded0(false);
    std::pair<uint16_t, uint16_t> decoded_ch0;
    auto const
        &ain0 = decoded_ch0.first,
        &bin0 = decoded_ch0.second;

#ifdef ADC_FOUR_CHANNEL
    uint32_t ch1(0);
    bool decoded1(false);
    std::pair<uint16_t, uint16_t> decoded_ch1;
    auto const &ain1 = decoded_ch1.first,
               &bin1 = decoded_ch1.second;
#endif

    void read() {
        static IMXRT_LPSPI_t *const spi_regs = &IMXRT_LPSPI4_S;
        if (!(life--)) {
            spi_regs->TCR |= LPSPI_TCR_TXMSK;
            while (spi_regs->RSR & LPSPI_RSR_RXEMPTY)
                ;
#ifdef ADC_FOUR_CHANNEL
            spi_regs->TCR |= LPSPI_TCR_TXMSK;
#endif
            ch0      = spi_regs->RDR;
            decoded0 = false;
#ifdef ADC_FOUR_CHANNEL
            while (spi_regs->RSR & LPSPI_RSR_RXEMPTY)
                ;
            ch1      = spi_regs->RDR;
            decoded1 = false;
#endif
            life = READ_LIFE;
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

uint16_t read_bin0() {
    read();
    if (!decoded0) {
        decoded_ch0 = decode(ch0);
        decoded0    = true;
    }
    return bin0;
}

#ifdef ADC_FOUR_CHANNEL
uint16_t read_ain1() {
    read();
    if (!decoded1) {
        decoded_ch1 = decode(ch1);
        decoded1    = true;
    }
    return ain1;
}

uint16_t read_bin1() {
    read();
    if (!decoded1) {
        decoded_ch1 = decode(ch1);
        decoded1    = true;
    }
    return bin1;
}
#endif