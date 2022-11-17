#ifndef REFERENCE_HPP
#define REFERENCE_HPP

#include <Arduino.h> 
#include <vector> 

struct Reference
{
    static elapsedMicros timer;

    size_t cur_slice, tot;
    double idle_val;
    std::vector<double> time_data, val_data;

    Reference(int sz, double const* time, double const* val, double idle_val=32768)
        : cur_slice(1),
          tot(sz),
          idle_val(idle_val),
          time_data(time, time + sz),
          val_data(val, val + sz)
    {
    }

    static void clear_timer();
    void clear_reference();    
    double get_reference();
    bool is_terminated() const;
};

template<int N>
Reference make_reference_from_array(double const (&time)[N], double const(&val) [N]) {
    return Reference(N, time, val);
}
#endif 