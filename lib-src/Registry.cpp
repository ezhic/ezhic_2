#include "Registry.h"
#include "EventWrapper.h"
#include "Timer.h"

using namespace ezhic;

// static
Registry *Registry::s_registry(0);
const Label Registry::DEFAULT_BUNDLE("_DEFAULT_BUNDLE@Registry");

Circuit &Registry::circuit(const Label &label)
{
    return d_circuits[label];
}

Registry::Registry() : d_timer(new Timer())
{
    writeBundle(DEFAULT_BUNDLE, Bundle());
}

void Registry::acceptUpdate(const EventWrapper *task)
{
    d_circuits[task->label()].acceptUpdate(task);
}

Registry &Registry::cloneInTimer(const Timer &timer)
{
    timer.clone().swap(d_timer);
    return *this;
}

const Bundle &Registry::writeBundle(const Label &label, const Bundle &b)
{
    Bundle &target = d_bundles[label];
    target = b;
    circuit(label).clear();
    return target;
}

Registry &Registry::instance()
{
    if (s_registry)
        return *s_registry;
    s_registry = new Registry();
    return *s_registry;
}

const Bundle &Registry::bundle(const Label &label)
{
    auto it = d_bundles.find(label);
    if (it == d_bundles.end()) {
        it = d_bundles.insert(std::make_pair(label, bundle(DEFAULT_BUNDLE)))
                 .first;
    }
    return it->second;
}

const Timer &Registry::timer() const
{
    return *d_timer;
}

namespace ezhic {
namespace ezreg {
const Timer &timer()
{
    return Registry::instance().timer();
}
void cloneInTimer(const Timer &timer)
{
    Registry::instance().cloneInTimer(timer);
}
const Bundle &getBundle(const Label &label)
{
    return Registry::instance().bundle(label);
}
const Bundle &writeBundle(const Label &label, const Bundle &b)
{
    return Registry::instance().writeBundle(label, b);
}
Circuit &circuit(const Label &label)
{
    return Registry::instance().circuit(label);
}
}
}