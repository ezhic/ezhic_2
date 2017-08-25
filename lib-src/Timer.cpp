#include <Timer.h>
#include <chrono>

namespace ezhic {

size_t Timer::now() const
{
    using namespace std::chrono;
    return duration_cast<nanoseconds>(steady_clock::now().time_since_epoch())
        .count();
}

std::unique_ptr<Timer> Timer::clone() const
{
    return std::unique_ptr<Timer>{new Timer()};
}
}
