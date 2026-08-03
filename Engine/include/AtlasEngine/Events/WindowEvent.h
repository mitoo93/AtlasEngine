#pragma once

#include "AtlasEngine/Events/Event.h"

namespace AtlasEngine
{

class WindowCloseEvent : public Event
{
public:

    static EventType GetStaticType()
    {
        return EventType::WindowClose;
    }

    EventType GetEventType() const override
    {
        return GetStaticType();
    }

    const char* GetName() const override
    {
        return "WindowClose";
    }
};

class WindowResizeEvent : public Event
{
public:

    WindowResizeEvent(
        unsigned int width,
        unsigned int height)
        : m_Width(width),
          m_Height(height)
    {
    }

    static EventType GetStaticType()
    {
        return EventType::WindowResize;
    }

    EventType GetEventType() const override
    {
        return GetStaticType();
    }

    const char* GetName() const override
    {
        return "WindowResize";
    }

    unsigned int GetWidth() const
    {
        return m_Width;
    }

    unsigned int GetHeight() const
    {
        return m_Height;
    }

private:

    unsigned int m_Width;
    unsigned int m_Height;
};

}