#ifndef PLAYGROUND_VIEWPORT_WINDOW_H
#define PLAYGROUND_VIEWPORT_WINDOW_H

#include "viewport_window.h"
#include "../Shaders/shader.h"
#include "../Entities/global_grid.h"

#include <map>
#include <cmath>

class PlaygroundViewportWindow : public ViewportWindow {
public:
    PlaygroundViewportWindow(glm::vec2 fractionalWindowDimensions, glm::vec2 fractionalPosition,
        const glm::ivec2& screenDimensions, Shader& shader)
        : ViewportWindow(fractionalWindowDimensions, fractionalPosition, screenDimensions, shader) {
        
    }
    ~PlaygroundViewportWindow() override {
        Logger::GetInstance()->info("Deleted Playground Viewport");
    }
    
    void SetGrid(GlobalGrid* g) {
        grid = g;
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
            {
                MouseButtonPressedEvent* mEvent = dynamic_cast<MouseButtonPressedEvent*>(&event);
                auto p = GetGridCoords(glm::vec2(mEvent->GetX(), mEvent->GetY()));
                std::cout << p.first << ", " << p.second << std::endl;
                break;
            }
            case EventType::MouseButtonReleased:
                break;
            case EventType::MouseMoved:
                break;
            case EventType::MouseScrolled:
                break;
                
        }
    }
    
    glm::vec2 GetGridPointPosition(int i, int j) const {
        glm::vec3 pos = grid->GetPosition();
        float sSpacing = grid->GetSquareSpacing();
        return (glm::vec2(pos.x, pos.y) + glm::vec2(j * sSpacing, i * sSpacing));
    }
    
    std::pair<unsigned int, unsigned int> GetGridCoords(glm::vec2 position) const {
        glm::vec3 pos = grid->GetPosition();
        float sSpacing = grid->GetSquareSpacing();
        
        float i = (position.y - pos.y ) / sSpacing;
        float j = (position.x - pos.x ) / sSpacing;
        
        return std::make_pair<unsigned int,unsigned int>(round(i), round(j));
    }
    
private:
    GlobalGrid* grid;
    
};

#endif
