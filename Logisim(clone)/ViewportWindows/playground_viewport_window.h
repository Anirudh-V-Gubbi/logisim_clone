#ifndef PLAYGROUND_VIEWPORT_WINDOW_H
#define PLAYGROUND_VIEWPORT_WINDOW_H

#include "viewport_window.h"
#include "../Entities/global_grid.h"
#include "../Entities/gates/and_gate_entity.h"
#include "../Entities/gates/not_gate_entity.h"
#include "../Entities/gates/nand_gate_entity.h"
#include "../Entities/gates/or_gate_entity.h"
#include "../Entities/gates/nor_gate_entity.h"
#include "../Entities/gates/xor_gate_entity.h"
#include "../Entities/gates/xnor_gate_entity.h"
#include "../Entities/wires/wire_entity.h"
#include "../Event/event_handler.h"
#include "../Entities/switches/input_switch_entity.h"
#include "../Entities/switches/output_switch_entity.h"

#include <map>
#include <cmath>

class PlaygroundViewportWindow : public ViewportWindow {
public:
    PlaygroundViewportWindow(glm::vec2 fractionalWindowDimensions, glm::vec2 fractionalPosition,
        const glm::ivec2& screenDimensions, Shader& shader)
        : ViewportWindow(fractionalWindowDimensions, fractionalPosition, screenDimensions, shader) {
            gateShader = new Shader("Shaders/gate_vertex.vs", "Shaders/gate_fragment.fs");
            wireShader = new Shader("Shaders/wire_vertex.vs", "Shaders/wire_fragment.fs");
            switchShader = new Shader("Shaders/switch_vertex.vs", "Shaders/switch_fragment.fs");
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
                auto m = grid->GetGridPointPosition(p.x, p.y);

                if(mEvent->GetMouseButton() != 0) {
                    AndGateEntity* gate = new AndGateEntity(*gateShader, glm::vec3(m.x - m_position.x * m_screenDimensions.x, m.y, 0.0f), glm::ivec2(p.x, p.y));
                    AddEntititesToViewport(*gate);
                }else {
                    
                    OutputSwitchEntity* oSwitch = new OutputSwitchEntity(*switchShader, glm::vec3(m.x - m_position.x * m_screenDimensions.x, m.y, 0.0f), glm::ivec2(p.x, p.y));
                    AddEntititesToViewport(*oSwitch);
                }
                
                std::cout << p.x << ", " << p.y << std::endl;
                break;
            }
            case EventType::MouseButtonReleased:
            {
                m_currentWire = nullptr;
                break;
            }
            case EventType::MouseMoved:
            {
                MouseMovedEvent* mEvent = dynamic_cast<MouseMovedEvent*>(&event);
                glm::vec2 currentPosition = glm::vec2(mEvent->GetX(), mEvent->GetY());

                auto startPoint = grid->GetGridCoords(EventHandler::clickPosition);
                auto currentPoint = grid->GetGridCoords(currentPosition);
                auto startPointPosition = grid->GetGridPointPosition(startPoint.x, startPoint.y) - glm::vec2(m_position.x * m_screenDimensions.x, 0.0f);
                auto currentPointPosition = grid->GetGridPointPosition(currentPoint.x, currentPoint.y) - glm::vec2(m_position.x * m_screenDimensions.x, 0.0f);
                
                if(EventHandler::MouseButtonsState[GLFW_MOUSE_BUTTON_LEFT] == ButtonState::PRESSED) {
                    if(m_currentWire == nullptr) {
                        if(startPoint.x != currentPoint.x || startPoint.y != currentPoint.y) {
                            WireEntity* wire = new WireEntity(*wireShader, glm::vec3(startPointPosition.x, startPointPosition.y, 0.0f));
                            wire->CreateWireFromPoints(startPoint, startPointPosition, currentPoint, currentPointPosition);
                            m_currentWire = wire;
                            AddEntititesToViewport(*wire);
                        }
                    }
                    else {
                        auto lastSocketPos = m_currentWire->GetLastSocket().GetPosition();
                        if(lastSocketPos.x != currentPoint.x || lastSocketPos.y != currentPoint.y) {
                            m_currentWire->ContinueBuildWire(currentPoint, currentPointPosition);
                        }
                        
                    }
                }
            }
                break;
            case EventType::MouseScrolled:
                break;
                
        }
    }
    
private:
    GlobalGrid* grid;
    Shader* gateShader;
    Shader* wireShader;
    Shader* switchShader;
    
    WireEntity* m_currentWire = nullptr;
};

#endif
