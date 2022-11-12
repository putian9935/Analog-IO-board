#ifndef PIN_ASSIGNMENT_H
#define PIN_ASSIGNMENT_H

/***
 * @brief pins for ADC communication 
 * 
 * MOSI, 11, B0_02
 * MISO, 12, B0_01
 * CS2, 36, B1_02
*/
#define CS 10
#define MOSI 11
#define MISO 12
#define SCK 13
#define CS2 36

/**
 * @brief pins for DAC communication 
 * 
 */
#define LDAC1 A10
#define DAC_CLR1 A11

#define LDAC2 29
#define DAC_CLR2 30

#define CS1 0
#define MISO1 1 
#define MOSI1 26
#define SCK1 27

#endif 