#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include "event.h"

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y)
        : m_mouseX(x), m_mouseY(y)
    {

    }

    inline float GetX() { return m_mouseX; }
    inline float GetY() { return m_mouseY; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    EVENT_CLASS_TYPE(MouseMoved)

private:
    float m_mouseX, m_mouseY;
};

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : m_xOffset(xOffset), m_yOffset(yOffset)
    {

    }

    inline float GetXOffset() { return m_xOffset; }
    inline float GetYOffset() { return m_yOffset; }
    
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    EVENT_CLASS_TYPE(MouseScrolled)

private:
    float m_xOffset, m_yOffset;
};

class MouseButtonEvent : public Event
{
public:
    inline int GetMouseButton() const { return m_button; }
    inline float GetX() { return m_mouseX; }
    inline float GetY() { return m_mouseY; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)
protected:
    MouseButtonEvent(int button, float x, float y)
        : m_button(button), m_mouseX(x), m_mouseY(y)
    {

    }

    int m_button;
    float m_mouseX, m_mouseY;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button, float x, float y)
        : MouseButtonEvent(button, x, y)
    {

    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button, float x, float y)
        : MouseButtonEvent(button, x, y)
    {

    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

#endif
