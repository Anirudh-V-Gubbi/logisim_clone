#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <Logger/logger.h>
#include <PubSub/publisher.h>
#include "application_event.h"
#include "key_event.h"
#include "mouse_event.h"

enum class ButtonState { RELEASED, PRESSED };

class EventHandler : public Publisher<Event> {
public:
  ~EventHandler() {}

  inline static std::map<GLint, ButtonState> MouseButtonsState = {
      {GLFW_MOUSE_BUTTON_LEFT, ButtonState::RELEASED},
      {GLFW_MOUSE_BUTTON_RIGHT, ButtonState::RELEASED},
      {GLFW_MOUSE_BUTTON_MIDDLE, ButtonState::RELEASED},
  };
  inline static glm::vec2 clickPosition;

  static std::shared_ptr<EventHandler> GetInstance() {
    if (m_instance == nullptr) {
      m_instance = std::make_shared<EventHandler>(EventHandler());
    }

    return m_instance;
  }

  void DispatchEvent(Event &event) {
    EventType type = event.GetEventType();

    switch (type) {
    case EventType::None:
      break;
    case EventType::WindowClose:
      break;
    case EventType::WindowResize:
      break;
    case EventType::KeyPressed:
      break;
    case EventType::KeyReleased:
      break;
    case EventType::KeyTyped:
      break;
    case EventType::MouseButtonPressed: {
      MouseButtonPressedEvent *mEvent =
          dynamic_cast<MouseButtonPressedEvent *>(&event);
      MouseButtonsState[mEvent->GetMouseButton()] = ButtonState::PRESSED;
      clickPosition.x = mEvent->GetX();
      clickPosition.y = mEvent->GetY();
      break;
    }
    case EventType::MouseButtonReleased: {
      MouseButtonReleasedEvent *mEvent =
          dynamic_cast<MouseButtonReleasedEvent *>(&event);
      MouseButtonsState[mEvent->GetMouseButton()] = ButtonState::RELEASED;
      break;
    }
    case EventType::MouseMoved:
      break;
    case EventType::MouseScrolled:
      break;
    }

    Notify(event);
  }

private:
  inline static std::shared_ptr<EventHandler> m_instance;

  EventHandler() {}
};


inline std::ostream& operator<<(std::ostream& os, const Event& event) {
    auto type = event.GetEventType();
    switch (type) {
      case EventType::None:
          os << "None";
          break;
      case EventType::WindowClose:
          os << "Window Close";
          break;
      case EventType::WindowResize:
      {
          auto dEvent = dynamic_cast<const WindowResizeEvent*>(&event);
          os << "Window Resize " << dEvent->GetWidth() << ", " << dEvent->GetHeight();
          break;
      }
      case EventType::KeyPressed:
      {
          auto dEvent = dynamic_cast<const KeyPressedEvent*>(&event);
          os << "Key Pressed " << dEvent->GetKeyCode() << "(" << dEvent->GetRepeatCount() << ")";
          break;
      }
      case EventType::KeyReleased:
      {
          auto dEvent = dynamic_cast<const KeyReleasedEvent*>(&event);
          os << "Key Released " << dEvent->GetKeyCode();
          break;
      }
      case EventType::KeyTyped:
          os << "Key Typed";
          break;
      case EventType::MouseButtonPressed:
      {
          auto dEvent = dynamic_cast<const MouseButtonPressedEvent*>(&event);
          os << "Mouse Button Pressed " << dEvent->GetMouseButton() << " " << dEvent->GetX() << ", " << dEvent->GetY();
          break;
      }
      case EventType::MouseButtonReleased:
      {
          auto dEvent = dynamic_cast<const MouseButtonReleasedEvent*>(&event);
          os << "Mouse Button released " << dEvent->GetMouseButton() << " " << dEvent->GetX() << ", " << dEvent->GetY();
          break;
      }
      case EventType::MouseMoved:
      {
          auto dEvent = dynamic_cast<const MouseMovedEvent*>(&event);
          os << "Mouse Moved " << dEvent->GetX() << ", " << dEvent->GetY();
          break;
      }
      case EventType::MouseScrolled:
      {
          auto dEvent = dynamic_cast<const MouseScrolledEvent*>(&event);
          os << "Mouse Scrolled " << dEvent->GetXOffset() << ", " << dEvent->GetYOffset();
          break;
      }
    }

    return os;
}

#endif
