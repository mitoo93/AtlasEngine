#pragma once

#include <string>

namespace AtlasEngine
{

enum class EventType
{
    None = 0,

    WindowClose,
    WindowResize,

    KeyPressed,
    KeyReleased,

    MouseMoved,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseScrolled
};

class Event
{
public:

    virtual ~Event() = default;

    virtual EventType GetEventType() const = 0;

    virtual const char* GetName() const = 0;

    bool Handled() const
    {
        return m_Handled;
    }

    void SetHandled(bool handled)
    {
        m_Handled = handled;
    }

private:

    bool m_Handled = false;
};

}