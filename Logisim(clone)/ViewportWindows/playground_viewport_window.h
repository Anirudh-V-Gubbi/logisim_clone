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
#include "../Entities/gates/socket.h"
#include "../Entities/wires/wire_entity.h"

#include <map>
#include <cmath>

class PlaygroundViewportWindow : public ViewportWindow {
public:
    PlaygroundViewportWindow(glm::vec2 fractionalWindowDimensions, glm::vec2 fractionalPosition,
        const glm::ivec2& screenDimensions, Shader& shader)
        : ViewportWindow(fractionalWindowDimensions, fractionalPosition, screenDimensions, shader) {
            gateShader = new Shader("Shaders/gate_vertex.vs", "Shaders/gate_fragment.fs");
            wireShader = new Shader("Shaders/wire_vertex.vs", "Shaders/wire_fragment.fs");
    }
    ~PlaygroundViewportWindow() override {
        // Delete gate resources
        glDeleteVertexArrays(1, &GateEntity::m_VAO);
        glDeleteBuffers(1, &GateEntity::m_VBO);
        glDeleteBuffers(1, &GateEntity::m_EBO);
        glDeleteProgram(gateShader->ID);
        delete gateShader;
        
        // Delete wire resources
        glDeleteVertexArrays(1, &WireEntity::VAO);
        glDeleteBuffers(1, &WireEntity::VBO);
        glDeleteBuffers(1, &WireEntity::EBO);
        glDeleteProgram(wireShader->ID);
        delete wireShader;

        // Delete gate socket resources
        glDeleteVertexArrays(1, &GateSockets::m_VAO);
        glDeleteBuffers(1, &GateSockets::m_VBO);
        glDeleteBuffers(1, &GateSockets::m_EBO);
        glDeleteTextures(1, &GateSockets::texture->ID);
        glDeleteProgram(GateSockets::shader->ID);
        delete GateSockets::shader;
        delete GateSockets::texture;
        
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
                auto p = grid->GetGridCoords(glm::vec2(mEvent->GetX(), mEvent->GetY()));
                auto m = grid->GetGridPointPosition(p.first, p.second);

                /*AndGateEntity* gate = new AndGateEntity(*gateShader, glm::vec3(m.x - m_position.x * m_screenDimensions.x, m.y, 0.0f), glm::ivec2(p.first, p.second));
                AddEntititesToViewport(*gate);*/
                
                WireEntity* wire = new WireEntity(*wireShader, glm::vec3(m.x - m_position.x * m_screenDimensions.x, m.y, 0.0f));
                auto q = glm::vec3(m.x - m_position.x * m_screenDimensions.x, m.y, 0.0f);
                wire->AddSocket(Socket(glm::ivec2(p.first, p.second), GlobalGrid::GetGrid()->GetGridPointPositionRelative(q, 0, 0)));
                wire->AddSocket(Socket(glm::ivec2(p.first + 1, p.second), GlobalGrid::GetGrid()->GetGridPointPositionRelative(q, 0, 1)));
                AddEntititesToViewport(*wire);
                
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
    
private:
    GlobalGrid* grid;
    Shader* gateShader;
    Shader* wireShader;
};

#endif
