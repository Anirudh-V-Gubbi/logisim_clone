#ifndef PLAYGROUND_VIEWPORT_WINDOW_H
#define PLAYGROUND_VIEWPORT_WINDOW_H

#include "viewport_window.h"
#include "../Shaders/shader.h"
#include "../Entities/global_grid.h"
#include "../Entities/gates/and_gate_entity.h"
#include "../Entities/gates/not_gate_entity.h"
#include "../Entities/gates/nand_gate_entity.h"
#include "../Entities/gates/or_gate_entity.h"
#include "../Entities/gates/nor_gate_entity.h"
#include "../Entities/gates/xor_gate_entity.h"
#include "../Entities/gates/xnor_gate_entity.h"

#include <map>
#include <cmath>

class PlaygroundViewportWindow : public ViewportWindow {
public:
    PlaygroundViewportWindow(glm::vec2 fractionalWindowDimensions, glm::vec2 fractionalPosition,
        const glm::ivec2& screenDimensions, Shader& shader)
        : ViewportWindow(fractionalWindowDimensions, fractionalPosition, screenDimensions, shader) {
            gateShader = new Shader("Shaders/gate_vertex.vs", "Shaders/gate_fragment.fs");
    }
    ~PlaygroundViewportWindow() override {
        glDeleteVertexArrays(1, &GateEntity::m_VAO);
        glDeleteBuffers(1, &GateEntity::m_VBO);
        glDeleteBuffers(1, &GateEntity::m_EBO);
        glDeleteProgram(gateShader->ID);
        delete gateShader;
        
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
                auto m = GetGridPointPosition(p.first, p.second);

                AndGateEntity* gate = new AndGateEntity(*gateShader, glm::vec3(m.x - m_position.x * m_screenDimensions.x, m.y, 0.0f), glm::ivec2(p.first, p.second));
                AddEntititesToViewport(*gate);
                
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
    Shader* gateShader;
};

#endif
