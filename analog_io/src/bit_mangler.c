#include "bit_mangler.h"


uint32_t encode_8_32(uint32_t num)
{
    num = ((num & 0x0000000F) | ((num & 0x000000F0) << 12));
    num = ((num & 0x00030003) | ((num & 0x000C000C) << 6));
    num = ((num & 0x01010101) | ((num & 0x02020202) << 3));
    return num;
}

