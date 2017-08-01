#ifndef INCLUDED_TIMER
#define INCLUDED_TIMER

#include <cstddef>

namespace ezhic {
// interface for injecting timers
// before c++11 we have no standard cross-platform high res timer.
class Timer
{
  public:
    virtual ~Timer() {}
    virtual size_t now() const;
    virtual Timer *clone() const;
};
}
#endif  // EZHIC_TIMER_H
