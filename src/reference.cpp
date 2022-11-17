#include "reference.hpp"

static inline double interp1d(double x1, double x2, double y1, double y2, double x)
{
    return (y2 - y1) / (x2 - x1) * (x - x1) + y1;
}

elapsedMicros Reference::timer = 0;

void Reference::clear_timer()
{
    Reference::timer = 0;
}

void Reference::clear_reference()
{
    cur_slice = 1;
}

bool Reference::is_terminated() const
{
    return cur_slice >= tot;
}

double Reference::get_reference()
{
    // already ended
    if (is_terminated())
        return idle_val;
    unsigned long cur_time = timer;

    // already ended
    if (cur_time > time_data[tot - 1])
    {
        cur_slice = tot;
        return idle_val;
    }
    while (cur_slice < tot && time_data[cur_slice] < cur_time)
        ++cur_slice;

    // note, cur_slice is always one past
    return interp1d(time_data[cur_slice - 1], time_data[cur_slice], val_data[cur_slice - 1], val_data[cur_slice], cur_time);
}
