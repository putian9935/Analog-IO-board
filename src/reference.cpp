#include "reference.hpp"
#include "serial_reader.hpp"

ReferencePath zero_reference;
static inline double interp1d(double x1, double x2, double y1, double y2, double x) {
    return (y2 - y1) / (x2 - x1) * (x - x1) + y1;
}

elapsedMicros ReferencePath::timer = 0;

void ReferencePath::clear_timer() {
    ReferencePath::timer = 0;
}

void ReferencePath::clear_reference() {
    cur_slice = 1;
}

bool ReferencePath::is_terminated() const {
    return cur_slice >= tot;
}

double ReferencePath::get_reference() {
    // already ended
    if (is_terminated())
        return idle_val;
    unsigned long cur_time = timer;

    // already ended
    if (cur_time > time_data[tot - 1]) {
        cur_slice = tot;
        return idle_val;
    }
    while (cur_slice < tot && time_data[cur_slice] < cur_time)
        ++cur_slice;

    // note, cur_slice is always one past
    return interp1d(time_data[cur_slice - 1], time_data[cur_slice], val_data[cur_slice - 1], val_data[cur_slice], cur_time);
}

void ReferencePath:: set_data_from_serial()  {
    uint32_t sz = SerialReader();
    sz /= 6;
    tot = sz;
    time_data.resize(sz,0);
    val_data.resize(sz, 0);
    auto it_time = time_data.begin();
    auto it_val = val_data.begin();
    while(sz--) {
        *it_time++ = SerialReader();
        *it_val++ = SerialReader();
    }
    idle_val = val_data[0];
}