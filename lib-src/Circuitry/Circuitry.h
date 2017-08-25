#ifndef INCLUDED_CIRCUITRY
#define INCLUDED_CIRCUITRY

#include "Event.h"

namespace ezhic {

namespace Circuitry {
class TracePredicate
{
  public:
    virtual ~TracePredicate() {}
    // Const as circuitry (breakers, menders, probers) should be stateless.
    virtual bool operator()(const EventTrace &history) const { return false; }
    virtual std::unique_ptr<TracePredicate> clone() const = 0;
};

template <typename Derived>
class TracePredicateCloneable : public TracePredicate
{
  public:
    virtual std::unique_ptr<TracePredicate> clone() const
    {
        return std::unique_ptr<TracePredicate>{
            new Derived(static_cast<Derived const &>(*this))};
    }
};

// ShouldMend: check if a broken circuit should be repaired
// ShouldProbe: check if the unreliable task may be probed
// ShouldBreak: check if the state of failures for a label is intolerable
}
}

#endif  // EZHIC_CIRCUITRY_H
