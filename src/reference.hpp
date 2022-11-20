#ifndef REFERENCE_HPP
#define REFERENCE_HPP

#include <Arduino.h>
#include <vector>

struct ReferenceBase;
typedef double (ReferenceBase::*read_mfunc_t)();

struct ReferenceBase {
    virtual double get_reference() = 0;
    virtual void set_data_from_serial() = 0; 
};

struct ReferenceConst : ReferenceBase {
    double ref;
    ReferenceConst(double const ref) : ref(ref) {}
    double get_reference() override {
        return ref;
    }
    void set_data_from_serial() override {};
};

extern ReferenceConst zero_reference;

struct ReferencePath : ReferenceBase {
    static elapsedMicros timer;

    size_t cur_slice, tot;
    uint16_t idle_val;
    std::vector<uint32_t> time_data;
    std::vector<uint16_t> val_data;

    ReferencePath() : cur_slice(1), tot(0), idle_val(0.) {}
    ReferencePath(int sz, uint32_t const* time, uint16_t const* val, uint16_t idle_val = 32768)
        : cur_slice(1),
          tot(sz),
          idle_val(idle_val),
          time_data(time, time + sz),
          val_data(val, val + sz) {
    }

    static void clear_timer();
    void clear_reference();
    double get_reference() override;
    bool is_terminated() const;
    void set_data_from_serial() override;
};

extern ReferencePath ref_410_master;

template <int N>
ReferencePath make_reference_from_array(double const (&time)[N], double const (&val)[N]) {
    return ReferencePath(N, time, val);
}
#endif