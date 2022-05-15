#include "bit_mangler.h"

typedef union {
    struct {uint32_t high,  low;}; 
    uint64_t full;
} new_64;



uint32_t insert_zeros_32(uint32_t num) {
    // assuming high 32-bits are 0 
    num = (num & 0x000000FF) | ((num & 0x0000FF00) << 8);
    num = (num & 0x000F000F) | ((num & 0x00F000F0) << 4);
    num = (num & 0x03030303) | ((num & 0x0C0C0C0C) << 2);
    num = (num & 0x55555555) | ((num & 0xAAAAAAAA) << 1);
    return num; 
}


uint64_t insert_zeros(uint32_t num) {
    // assuming high 32-bits are 0 
    new_64 ret; 
    ret.full =  (num & 0x000000000000FFFF) | ((uint64_t) (num & 0xFFFF0000) << 16);
    ret.low = insert_zeros_32(ret.low);
    ret.high = insert_zeros_32(ret.high);
    return ret.full;
}

uint64_t insert_zeros_old(uint64_t num) {
    // assuming high 32-bits are 0 
    num = (num & 0x000000000000FFFF) | ((num & 0x00000000FFFF0000) << 16);
    num = (num & 0x000000FF000000FF) | ((num & 0x0000FF000000FF00) << 8);
    num = (num & 0x000F000F000F000F) | ((num & 0x00F000F000F000F0) << 4);
    num = (num & 0x0303030303030303) | ((num & 0x0C0C0C0C0C0C0C0C) << 2);
    num = (num & 0x5555555555555555) | ((num & 0xAAAAAAAAAAAAAAAA) << 1);
    return num;
}



