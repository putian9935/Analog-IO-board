/**
 * @file pi_controller.hpp
 * @author Li Putian
 * @brief Example of controller usage, simple PI controller
 * @date 2023-04-15
 * 
 * 
 */
#ifndef PI_CONTROLLER_HPP
#define PI_CONTROLLER_HPP

#include "controller.hpp"

using PIController = IIRCascadeController<1, 2>;
void servo_parser(PIController*);
void sweep_parser(PIController*);
void show_parser(PIController*);

#endif 