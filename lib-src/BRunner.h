#ifndef INCLUDED_BRUNNER_H
#define INCLUDED_BRUNNER_H

#include "Bundle.h"
#include "Circuitry/Circuit.h"
#include "Handles.h"
#include "Timer.h"

namespace ezhic {
struct BRunner
{
    // will use global registry to initialize
    explicit BRunner(const Label &label);
    // will operate on user-owned circuit, bundle, timer
    BRunner(Circuit &c, const Bundle &b, Timer *t);
    virtual ~BRunner() {}
    const Label d_label;
    Circuit &d_circuit;
    const Bundle &d_bundle;
    const Timer *d_timer;

    // allow intercepting type-erased result for post-run in derived classes
    // we pledge to call it for each run (after updating acceptors)
    virtual void postRun(TypeErasedBase &result) {}

    template <typename Callable, typename... Args>
    auto run(Callable callable, Args &&... args)
        -> TNullable<typename std::result_of<Callable(Args...)>::type>;

    template <typename Callable, typename Obj, typename... Args>
    auto run_m(Obj obj, Callable callable, Args &&... args)
        -> TNullable<typename std::result_of<Callable(Obj, Args...)>::type>;
};
}

#include "BRunner_IMP.h"

#endif  // EZHIC_BRUNNER_H
