#ifndef PLAYGROUND_VIEWPORT_WINDOW_H
#define PLAYGROUND_VIEWPORT_WINDOW_H

#include "viewport_window.h"
#include <Entities/global_grid.h>
#include <Entities/gates/and_gate_entity.h>
#include <Entities/gates/not_gate_entity.h>
#include <Entities/gates/nand_gate_entity.h>
#include <Entities/gates/or_gate_entity.h>
#include <Entities/gates/nor_gate_entity.h>
#include <Entities/gates/xor_gate_entity.h>
#include <Entities/gates/xnor_gate_entity.h>
#include <Entities/wires/wire_entity.h>
#include <Entities/switches/input_switch_entity.h>
#include <Entities/switches/output_switch_entity.h>
#include <Shaders/shader_manager.h>
#include <Logger/log.h>

#include <map>
#include <cmath>
#include <memory>

class PlaygroundViewportWindow : public ViewportWindow {
public:
    PlaygroundViewportWindow(glm::vec2 fractionalWindowDimensions, glm::vec2 fractionalPosition,
        const glm::ivec2& screenDimensions, std::shared_ptr<Shader> shader)
        : ViewportWindow(fractionalWindowDimensions, fractionalPosition, screenDimensions, shader) {
            LOG_FUNCTION(this, fractionalWindowDimensions, fractionalPosition, screenDimensions, shader);
    }
    ~PlaygroundViewportWindow() override {
        LOG_FUNCTION(this);

        // Delete gate resources
        glDeleteVertexArrays(1, &GateEntity::m_VAO);
        glDeleteBuffers(1, &GateEntity::m_VBO);
        glDeleteBuffers(1, &GateEntity::m_EBO);

        // Delete wire resources
        glDeleteVertexArrays(1, &WireEntity::VAO);
        glDeleteBuffers(1, &WireEntity::VBO);
        glDeleteBuffers(1, &WireEntity::EBO);

        // Delete gate socket resources
        if(GateSockets::m_VAO != 0) {
            glDeleteVertexArrays(1, &GateSockets::m_VAO);
            glDeleteBuffers(1, &GateSockets::m_VBO);
            glDeleteBuffers(1, &GateSockets::m_EBO);
            glDeleteTextures(1, &GateSockets::texture->ID);
            delete GateSockets::texture;
        }
        
        Logger::GetInstance()->info("Deleted Playground Viewport");
    }
    
    void SetGrid(std::shared_ptr<GlobalGrid> g) {
        LOG_FUNCTION(this, g);

         grid = g;
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
            {
                MouseButtonPressedEvent* mEvent = dynamic_cast<MouseButtonPressedEvent*>(&event);
                auto p = grid->GetGridCoords(glm::vec2(mEvent->GetX(), mEvent->GetY()));
                auto m = grid->GetGridPointPosition(p.x, p.y);

                if(mEvent->GetMouseButton() != 0) {
                    std::shared_ptr<AndGateEntity> gate = std::make_shared<AndGateEntity>(ShaderManager::GetShader("gate"),
                                                                                            glm::vec3(m.x - m_position.x * m_screenDimensions.x, m.y, 0.0f),
                                                                                            glm::ivec2(p.x, p.y));
                    AddEntititesToViewport(gate);
                }else {

                    std::shared_ptr<OutputSwitchEntity> oSwitch = std::make_shared<OutputSwitchEntity>(ShaderManager::GetShader("switch"),
                                                                                                        glm::vec3(m.x - m_position.x * m_screenDimensions.x, m.y, 0.0f),
                                                                                                        glm::ivec2(p.x, p.y));
                    AddEntititesToViewport(oSwitch);
                }

                Logger::GetInstance()->info(p.x, ',', p.y);
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
                            std::shared_ptr<WireEntity> wire = std::make_shared<WireEntity>(ShaderManager::GetShader("wire"),
                                                                                            glm::vec3(startPointPosition.x, startPointPosition.y, 0.0f));
                            wire->CreateWireFromPoints(startPoint, startPointPosition, currentPoint, currentPointPosition);
                            m_currentWire = wire.get();
                            AddEntititesToViewport(wire);
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
    std::shared_ptr<GlobalGrid> grid;
    
    WireEntity* m_currentWire = nullptr;
};

#endif
