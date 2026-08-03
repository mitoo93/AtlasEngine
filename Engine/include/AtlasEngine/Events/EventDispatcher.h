#pragma once

#include <functional>
#include "AtlasEngine/Events/Event.h"

namespace AtlasEngine
{

class EventDispatcher
{
public:

    EventDispatcher(Event& event)
        : m_Event(event)
    {
    }

    template<typename TEvent, typename F>
    bool Dispatch(const F& function)
    {
        if (m_Event.GetEventType() == TEvent::GetStaticType())
        {
            function(static_cast<TEvent&>(m_Event));

            return true;
        }

        return false;
    }

private:

    Event& m_Event;
};

}