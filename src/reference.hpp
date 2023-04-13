/**
 * @file reference.hpp
 * @brief Reference class for servo system.
 * @date 2022-11-28
 *
 *
 */
#ifndef REFERENCE_HPP
#define REFERENCE_HPP

#include <Arduino.h>
#include <vector>

struct ReferencePath {
    volatile bool trigged;
    size_t cur_slice, tot;
    uint16_t idle_val;
    elapsedMicros timer;
    std::vector<uint32_t> time_data;
    std::vector<uint16_t> val_data;

    ReferencePath() : trigged(false), cur_slice(0), tot(0), idle_val(0.), timer(0) {}
    ReferencePath(int sz, uint32_t const* time, uint16_t const* val, uint16_t idle_val = 32768)
        : trigged(false),
          cur_slice(0),
          tot(sz),
          idle_val(idle_val),
          timer(0),
          time_data(time, time + sz),
          val_data(val, val + sz) {
    }

    void clear_timer();
    void clear_reference();
    double get_reference();
    bool is_terminated() const;
    void set_data_from_serial();
};

extern ReferencePath zero_reference;

template <int N>
ReferencePath make_reference_from_array(double const (&time)[N], double const (&val)[N]) {
    return ReferencePath(N, time, val);
}
#endif
