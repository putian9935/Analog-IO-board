/**
 * @file bit_mangler.h
 * @brief Encode and decode bits for parallel SPI transfer
 * @date 2022-11-28
 * 
 * 
 */
#ifndef BIT_MANGLER_H

#include <Arduino.h> 

#ifdef __cplusplus
extern "C"{
#endif 

uint64_t insert_zeros(uint32_t num);
uint32_t encode_8_32(uint32_t num);
uint16_t decode_32_16(uint32_t num);

#ifdef __cplusplus
}
#endif

#endif 
