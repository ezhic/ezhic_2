#include "AcceptUpdates.h"
#include "EventWrapper.h"

namespace ezhic {

Event AcceptUpdates::acceptUpdate(const EventWrapper *task) const
{
    return task ? task->event() : Event();
}

TimeoutTagger::TimeoutTagger(Event::duration_t threshold)
: d_threshold(threshold) 
{
}

Event TimeoutTagger::acceptUpdate(const EventWrapper *task) const
{
    Event event = AcceptUpdates::acceptUpdate(task);
    if (event.hasKnownDuration() && event.duration() > d_threshold)
        event.type(Event::TIMEOUT);
    return event;
}

AcceptorsArray::AcceptorsArray(const AcceptorsArray &rhs) {
    for (const auto &acceptor : rhs.d_acceptors)
    {
        d_acceptors.push_back(acceptor->clone());
    }
}

void AcceptorsArray::cloneIn(const AcceptUpdates &acpt)
{
    d_acceptors.push_back(acpt.clone());
}

Event AcceptorsArray::acceptUpdate(const EventWrapper *task) const
{
    Event event = AcceptUpdates::acceptUpdate(task);
    for (const auto& acceptor : d_acceptors) {
        event = acceptor->acceptUpdate(task);
    }
    return event;
}

AcceptorsArray::~AcceptorsArray()
{
}
}
