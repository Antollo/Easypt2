#ifndef SRC_RANGE_H
#define SRC_RANGE_H

#include "number.h"

class range
{
public:
    range(number start, number stop, number step)
        : start(start), stop(stop), step(step) {}

    range(number start, number stop)
        : start(start), stop(stop), step(start < stop ? 1_n : -1_n) {}

    number getStart() const { return start; }
    number getStop() const { return stop; }
    number getStep() const { return step; }

    void setStart(number v) { start = v; }
    void setStop(number v) { stop = v; }
    void setStep(number v) { step = v; }

    number size() const { return ((stop - start) / step).ceil(); }
    number operator[](number i) { return start + step * i; }

private:
    number start, stop, step;
};

#endif /* SRC_RANGE_H */
