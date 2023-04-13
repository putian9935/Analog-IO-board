#include "reference.hpp"
#include "serial_reader.hpp"

ReferencePath zero_reference;
static double interp1d(double x1, double x2, double y1, double y2, double x) {
    return (y2 - y1) / (x2 - x1) * (x - x1) + y1;
}

void ReferencePath::clear_timer() {
    timer = 0;
}

void ReferencePath::clear_reference() {
    cur_slice = 0;
}

bool ReferencePath::is_terminated() const {
    return cur_slice >= tot;
}
#include "write.hpp"
double ReferencePath::get_reference() {
    if (trigged) {
        trigged = false;  // clear trigger flag
        if(timer > 20) {
          timer   = 0;
          cur_slice += 1;
          cur_slice %= tot;
        }
    }
    unsigned long cur_time = timer;

    size_t cur = cur_slice;
    if (cur_time > time_data[cur]) {
        //write(0, val_data[cur] - 32767);
        return val_data[cur];
    }
    // note, cur_slice is always one past
    uint16_t ret;
    if (cur == 0)
        ret = interp1d(0, time_data[cur], val_data[tot - 1], val_data[cur], cur_time);
    else
        ret = interp1d(0, time_data[cur], val_data[cur - 1], val_data[cur], cur_time);
        // write(0, ret - 32767);
    
    return ret;
}

void ReferencePath::set_data_from_serial() {
    uint32_t sz = SerialReader();
    sz /= 6;
    tot = sz;
    time_data.resize(sz, 0);
    val_data.resize(sz, 0);
    auto it_time = time_data.begin();
    auto it_val  = val_data.begin();
    while (sz--) {
        *it_time++ = SerialReader();
        *it_val++  = SerialReader();
    }
    idle_val = val_data[0];
}
