#include "bit_mangler.h"

typedef union {
    struct {uint32_t high,  low;}; 
    uint64_t full;
} new_64;

static uint32_t insert_zeros_32(uint32_t num) {
    // assuming high 16-bits are 0 
    num = (num & 0x000000FF) | ((num & 0x0000FF00) << 8);
    num = (num & 0x000F000F) | ((num & 0x00F000F0) << 4);
    num = (num & 0x03030303) | ((num & 0x0C0C0C0C) << 2);
    num = (num & 0x55555555) | ((num & 0xAAAAAAAA) << 1);
    return num; 
}

static uint32_t insert_zeros_32_high(uint32_t num) {
    // assuming high 24-bits are 0 
    num = (num & 0x000F000F) | ((num & 0x00F000F0) << 4);
    num = (num & 0x03030303) | ((num & 0x0C0C0C0C) << 2);
    num = (num & 0x55555555) | ((num & 0xAAAAAAAA) << 1);
    return num; 
}

uint64_t insert_zeros(uint32_t num) {
    // assuming high 32-bits are 0 
    new_64 ret; 
    ret.full =  (num & 0x000000000000FFFF) | ((uint64_t) (num & 0xFFFF0000) << 16);
    ret.low = insert_zeros_32_high(ret.low);
    ret.high = insert_zeros_32(ret.high);
    return ret.full;
}

uint32_t encode_8_32(uint32_t num)
{
    num = ((num & 0x0000000F) | ((num & 0x000000F0) << 12));
    num = ((num & 0x00030003) | ((num & 0x000C000C) << 6));
    num = ((num & 0x01010101) | ((num & 0x02020202) << 3));
    return num;
}

uint16_t decode_32_16(uint32_t num)
{
    num = (num & 0x11111111) | ((num & 0x44444444) >> 1);
    num = (num & 0x03030303) | ((num & 0x30303030) >> 2);
    num = (num & 0x000F000F) | ((num & 0x0F000F00) >> 4);
    num = (num & 0x000000FF) | ((num & 0x00FF0000) >> 8);
    return num;
}
