#ifndef TOOLBAR_VIEWPORT_WINDOW_H
#define TOOLBAR_VIEWPORT_WINDOW_H

#include "viewport_window.h"

class ToolbarViewportWindow : public ViewportWindow {
public:
    ToolbarViewportWindow(glm::vec2 fractionalWindowDimensions, glm::vec2 fractionalPosition,
        const glm::ivec2& screenDimensions, std::shared_ptr<Shader> shader)
        : ViewportWindow(fractionalWindowDimensions, fractionalPosition, screenDimensions, shader) {
            LOG_FUNCTION(this, fractionalWindowDimensions, fractionalPosition, screenDimensions, shader);
        
    }
    ~ToolbarViewportWindow() override {
        LOG_FUNCTION(this);
    }
    
    void HandleEvent(Event& event) override {
        LOG_FUNCTION(this, event);

        EventType type = event.GetEventType();
        int category = event.GetCategoryFlags();
        
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
                break;
            case EventType::MouseButtonReleased:
                break;
            case EventType::MouseMoved:
                break;
            case EventType::MouseScrolled:
                break;
                
        }
    }
};

#endif
