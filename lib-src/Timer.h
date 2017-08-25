#ifndef INCLUDED_TIMER
#define INCLUDED_TIMER

#include <cstddef>
#include <memory>

namespace ezhic {
// interface for injecting timers
class Timer
{
  public:
    virtual ~Timer() {}
    virtual size_t now() const;
    virtual std::unique_ptr<Timer> clone() const;
};
}
#endif  // EZHIC_TIMER_H
