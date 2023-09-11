/**
 * @file bit_mangler.h
 * @brief Encode and decode bits for parallel SPI transfer
 * @date 2022-09-11
 * 
 * 
 */
#ifndef BIT_MANGLER_H

#include <Arduino.h> 

#ifdef __cplusplus
extern "C"{
#endif 

uint32_t encode_8_32(uint32_t num);
__attribute__((always_inline))  inline uint16_t decode_32_16(uint32_t num)
{
    num = (num & 0x11111111) | ((num & 0x44444444) >> 1);
    num = (num & 0x03030303) | ((num & 0x30303030) >> 2);
    num = (num & 0x000F000F) | ((num & 0x0F000F00) >> 4);
    num = (num & 0x000000FF) | ((num & 0x00FF0000) >> 8);
    return num;
}
#ifdef __cplusplus
}
#endif

#endif 
