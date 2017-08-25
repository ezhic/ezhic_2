#include "IntentionTagger.h"
#include "CircuitPredicates.h"
#include <algorithm>

namespace ezhic {

struct TraceChecker
{  // just binds a history and executes functors with it
    TraceChecker(const EventTrace &history) : d_history(history) {}
    template <typename Predicate>
    bool operator()(const Predicate &predicate) const
    {
        return (*predicate)(d_history);
    }

    const EventTrace &d_history;
};

Event::Intention IntentionTagger::operator()(Circuit &circuit) const
{
    const EventTrace &eventTrace = circuit.eventTrace();
    TraceChecker traceChecker(eventTrace);
    Circuit::Health currentCircuitHealth = circuit.health();

    if (anyChecker(SHOULD_BREAK, traceChecker)) {
        circuit.health(Circuit::BAD);
        return Event::NO_RUN;
    }

    if (currentCircuitHealth == Circuit::GOOD)
        return Event::ROUTINE;

    // circuit state is bad
    if (anyChecker(SHOULD_MEND, traceChecker)) {
        circuit.health(Circuit::GOOD);
        return Event::ROUTINE;
    }

    if (anyChecker(SHOULD_PROBE, traceChecker)) {
        return Event::PROBE;
    }
    return Event::NO_RUN;
}

IntentionTagger::IntentionTagger(const IntentionTagger &rhs)
{
    for (unsigned t = 0; t != NONE; ++t)
        for (const auto& checker : rhs.d_checkers[t]) {
            d_checkers[t].push_back(checker->clone());
        }
}

IntentionTagger &IntentionTagger::operator=(const IntentionTagger &rhs)
{
    if (this != &rhs) {
        for (unsigned t = 0; t != NONE; ++t)
            for (const auto& checker : rhs.d_checkers[t]) {
                d_checkers[t].push_back(checker->clone());
            }
    }
    return *this;
}

void IntentionTagger::swap(IntentionTagger &rhs)
{
    for (unsigned t = 0; t != NONE; ++t)
        rhs.d_checkers[t].swap(d_checkers[t]);
}

IntentionTagger::~IntentionTagger()
{
}

IntentionTagger &
IntentionTagger::cloneIn(CheckerType type,
                         const Circuitry::TracePredicate &checker)
{
    if (type < NONE)
        d_checkers[type].push_back(checker.clone());
    return *this;
}

bool IntentionTagger::anyChecker(IntentionTagger::CheckerType type, const TraceChecker &traceChecker) const
{
    const auto& checkers = d_checkers[type];
    return std::any_of(checkers.cbegin(), checkers.cend(), traceChecker);
}
}
