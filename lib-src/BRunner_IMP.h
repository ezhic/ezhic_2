#ifndef INCLUDED_BUNDLE_IMP_H
#define INCLUDED_BUNDLE_IMP_H

#include "EventWrapper.h"
#include "Registry.h"
#include <algorithm>

#define __EZ_START()                                                          \
    Event::Intention intention =                                              \
        _local_bundle_for_scope.nextRunIntention(_local_circuit_for_scope);   \
    _dummy_task_for_scope.event().intention(intention);                       \
    bool shouldRun = intention != Event::NO_RUN;                              \
    if (shouldRun) {                                                          \
        try {                                                                 \
            _dummy_task_for_scope.start();

#define EZ_START(__execlabel)                                                 \
    {                                                                         \
        Registry &_local_registry_ref = Registry::instance();                 \
        const Bundle &_local_bundle_for_scope =                               \
            _local_registry_ref.bundle(__execlabel);                          \
        EventWrapper _dummy_task_for_scope(__execlabel);                      \
        _dummy_task_for_scope.timer(_local_registry_ref.timer());             \
        Circuit &_local_circuit_for_scope =                                   \
            _local_registry_ref.circuit(__execlabel);                         \
        __EZ_START()

#define EZ_END()                                                              \
    _dummy_task_for_scope.done();                                             \
    }                                                                         \
    catch (...)                                                               \
    {                                                                         \
        _dummy_task_for_scope.event().type(Event::EXCEPTION);                 \
        if (_local_bundle_for_scope.isRethrow()) {                            \
            _dummy_task_for_scope.event(_local_bundle_for_scope.acceptUpdate( \
                &_dummy_task_for_scope));                                     \
            _local_circuit_for_scope.acceptUpdate(&_dummy_task_for_scope);    \
            throw;                                                            \
        }                                                                     \
    } /* end try-catch */                                                     \
    } /* end should run */                                                    \
    else { _dummy_task_for_scope.putTimestamp(); }                            \
    _dummy_task_for_scope.event(                                              \
        _local_bundle_for_scope.acceptUpdate(&_dummy_task_for_scope));        \
    _local_circuit_for_scope.acceptUpdate(&_dummy_task_for_scope);            \
    }

namespace ezhic {

namespace b_assign_void_trick {
template <typename T>
inline T &passthrough(T &v)
{
    return v;
}

template <typename T>
inline void operator,(TNullable<T> &dest, const T &v)
{
    dest = v;
}
}

#define VCallInstrumentator(invocation)                                       \
    R result;                                                                 \
    {                                                                         \
        EventWrapper _dummy_task_for_scope(d_label);                          \
        _dummy_task_for_scope.timer(d_timer);                                 \
        Circuit &_local_circuit_for_scope = d_circuit;                        \
        const Bundle &_local_bundle_for_scope = d_bundle;                     \
        __EZ_START()                                                          \
        using namespace b_assign_void_trick;                                  \
        passthrough(result), invocation;                                      \
        EZ_END();                                                             \
        postRun(result);                                                      \
        return result;

template <typename Callable, typename... Args>
auto BRunner::run(Callable callable, Args &&... args)
    -> TNullable<typename std::result_of<Callable(Args...)>::type>
{
    using R = TNullable<typename std::result_of<Callable(Args...)>::type>;
    VCallInstrumentator(callable(args...));
}

template <typename Callable, typename Obj, typename... Args>
auto BRunner::run_m(Obj obj, Callable callable, Args &&... args)
    -> TNullable<typename std::result_of<Callable(Obj, Args...)>::type>
{
    using R = TNullable<typename std::result_of<Callable(Obj, Args...)>::type>;
    VCallInstrumentator(((obj).*(callable))(args...));
};
}

#endif  // EZHIC_BUNDLE_IMP_H
