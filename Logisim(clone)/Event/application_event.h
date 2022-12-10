#ifndef APPLICATION_EVENT_H
#define APPLICATION_EVENT_H

#include "event.h"

class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_width(width), m_height(height)
    {

    }

    inline unsigned int GetWidth() const { return m_width; }
    inline unsigned int GetHeight() const { return m_height; }

    EVENT_CLASS_CATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(WindowResize)

private:
    unsigned int m_width, m_height;
};

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent()
    {

    }

    EVENT_CLASS_CATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(WindowClose)
};

#endif
