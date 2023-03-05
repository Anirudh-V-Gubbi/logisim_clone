#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "../Logger/logger.h"
#include "key_event.h"
#include "mouse_event.h"
#include "application_event.h"
#include "../ViewportWindows/viewport_window.h"

enum class ButtonState{
    RELEASED,
    PRESSED
};

class EventHandler {
public:
    inline static std::map<GLint, ButtonState> MouseButtonsState = {
        {GLFW_MOUSE_BUTTON_LEFT, ButtonState::RELEASED},
        {GLFW_MOUSE_BUTTON_RIGHT, ButtonState::RELEASED},
        {GLFW_MOUSE_BUTTON_MIDDLE, ButtonState::RELEASED},
    };
    
    inline static glm::vec2 clickPosition;
    
    static void DispatchEvent(Event& event, std::vector<std::unique_ptr<ViewportWindow>>& viewportWindows) {
        EventType type = event.GetEventType();
        
        switch(type) {
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
            case EventType::MouseButtonPressed:
            {
                MouseButtonPressedEvent* mEvent = dynamic_cast<MouseButtonPressedEvent*>(&event);
                MouseButtonsState[mEvent->GetMouseButton()] = ButtonState::PRESSED;
                clickPosition.x = mEvent->GetX();
                clickPosition.y = mEvent->GetY();
                break;
            }
            case EventType::MouseButtonReleased:
            {
                MouseButtonReleasedEvent* mEvent = dynamic_cast<MouseButtonReleasedEvent*>(&event);
                MouseButtonsState[mEvent->GetMouseButton()] = ButtonState::RELEASED;
                break;
            }
            case EventType::MouseMoved:
                break;
            case EventType::MouseScrolled:
                break;
                
        }
        
        for(auto& vWindow : viewportWindows) {
            vWindow->HandleEvent(event);
        }
    }
};

#endif
