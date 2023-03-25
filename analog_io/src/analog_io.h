/**
 * @file analog_io.h
 * @author Li Putian
 * @brief Master header for analog IO library 
 * @version 0.1
 * @date 2023-03-25
 * 
 */

#ifndef ANALOG_IO_H
#define ANALOG_IO_H

#include "init_chips.hpp"
#include "write.hpp"
#include "read.hpp"

/**
 * @name Analog Read
 * 
 */


/**
 * @name Analog Write
 * 
 */
// @{
auto &analogio_write_both = write_both; 
auto &analogio_write = write; 
// @} 

/**
 * @name Digital Read
 * 
 */
// @{
auto &digitalio_read = digitalReadFast; 
auto &digitalio_write = digitalWriteFast; 
auto &digitalio_mode = pinMode; 
// @}

#endif 