#ifndef PROJECTSPACE_VIEWPORT_WINDOW_H
#define  PROJECTSPACE_VIEWPORT_WINDOW_H

#include "viewport_window.h"

class ProjectspaceViewportWindow : public ViewportWindow {
public:
    ProjectspaceViewportWindow(glm::vec2 fractionalWindowDimensions, glm::vec2 fractionalPosition,
        const glm::ivec2& screenDimensions, std::shared_ptr<Shader> shader)
        : ViewportWindow(fractionalWindowDimensions, fractionalPosition, screenDimensions, shader) {
        
    }
    ~ProjectspaceViewportWindow() override {
        Logger::GetInstance()->info("Deleted Projectspace Viewport");
    }
    
    void HandleEvent(Event& event) override {
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
