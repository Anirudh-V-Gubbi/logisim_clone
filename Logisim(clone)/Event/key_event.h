#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include "event.h"

class KeyEvent : public Event
{
public:
    inline int GetKeyCode() const { return m_keyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
    KeyEvent(int keycode) : m_keyCode(keycode)
    {

    }

    int m_keyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(int keycode, int repeatCount)
        : KeyEvent(keycode), m_repeatCount(repeatCount)
    {

    }

    inline int GetRepeatCount() const { return m_repeatCount; }

    EVENT_CLASS_TYPE(KeyPressed)
private:
        int m_repeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(int keycode)
        : KeyEvent(keycode)
    {

    }

    EVENT_CLASS_TYPE(KeyReleased)
};

#endif
