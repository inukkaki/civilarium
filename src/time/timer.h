#ifndef CIVILARIUM_TIME_TIMER_H_
#define CIVILARIUM_TIME_TIMER_H_

#include <cstdint>

namespace civilarium::time::timer {

class SimpleTimer {
public:
    SimpleTimer()
    {
        Set();
    }

    void Set();
    uint64_t GetElapsedTime() const;

private:
    uint64_t start_time_;
};

}  // namespace civilarium::time::timer

#endif  // CIVILARIUM_TIME_TIMER_H_
