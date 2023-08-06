#ifndef EVENT_H
#define EVENT_H

enum class EventType
{
    None = 0,
    WindowClose, WindowResize,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
    None = 0,
    EventCategoryApplication = 1,
    EventCategoryInput = 2,
    EventCategoryKeyboard = 4,
    EventCategoryMouse = 8,
    EventCategoryMouseButton = 16
};

#define EVENT_CLASS_TYPE(type) virtual EventType GetEventType() const override { return EventType::type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class Event {
public:
    virtual ~Event() = default;

    bool Handled = false;

    virtual EventType GetEventType() const = 0;
    virtual int GetCategoryFlags() const = 0;

    bool isInCategory(EventCategory category)
    {
        return (GetCategoryFlags() & category);
    }
protected:
    Event() {}
};

#endif
