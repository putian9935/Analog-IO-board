
#include "read.hpp"
#include "bit_mangler.h"
#include "adc_dma.hpp"


static std::pair<uint16_t, uint16_t> decode(uint32_t data)
{
    return std::pair<uint16_t, uint16_t>(decode_32_16(data >> 1), decode_32_16(data));
}

uint16_t read_ain0()
{
    return decode(ch0).first; 
}
uint16_t read_ain1()
{
    return decode(ch1).first; 
}
uint16_t read_bin0()
{
    return decode(ch0).second; 
}
uint16_t read_bin1()
{
    return decode(ch1).second; 
}