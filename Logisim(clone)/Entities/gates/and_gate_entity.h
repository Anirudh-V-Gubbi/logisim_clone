#ifndef AND_GATE_ENTITY_H
#define AND_GATE_ENTITY_H

#include "gate_entity.h"

class AndGateEntity : public GateEntity {
public:
    AndGateEntity(Shader& shader, glm::vec3 position, glm::ivec2 gridPosition)
    : GateEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        if(gate == NULL) {
            gate = parseScriptToGate("entity_and_gate.txt");
        }
        InitializeTexture();
        InitializeInputs();
        InitializeOutput();
    }
    ~AndGateEntity() { }
    
    AndGateEntity* GetInstance() const override {
        return (AndGateEntity*)this;
    }
    
    SocketState LogicFunction() override {
        return SocketState::UNINITIALIZED;
    }
    
private:
    inline static GateFromScript *gate = NULL;
    
    void InitializeTexture() {
        m_texture = gate->texture;
    }
    
    void InitializeInputs() {
        for(auto& [x, y] : gate->inputOffsets) {
            
            switch(m_direction) {
                case Direction::NORTH:
                    m_inputs.push_back(Socket(glm::ivec2(m_gridPosition.x + y, m_gridPosition.y - x)));
                    break;
                case Direction::SOUTH:
                    m_inputs.push_back(Socket(glm::ivec2(m_gridPosition.x - y, m_gridPosition.y + x)));
                    break;
                case Direction::EAST:
                    m_inputs.push_back(Socket(glm::ivec2(m_gridPosition.x + x, m_gridPosition.y + y)));
                    break;
                case Direction::WEST:
                    m_inputs.push_back(Socket(glm::ivec2(m_gridPosition.x - x, m_gridPosition.y - y)));
                    break;
            }
        }
    }
    
    void InitializeOutput() {
        int x = gate->outputOffset.first;
        int y = gate->outputOffset.second;
        
        switch(m_direction) {
            case Direction::NORTH:
                m_output = Socket(glm::ivec2(m_gridPosition.x + y, m_gridPosition.y - x));
                break;
            case Direction::SOUTH:
                m_output = Socket(glm::ivec2(m_gridPosition.x - y, m_gridPosition.y + x));
                break;
            case Direction::EAST:
                m_output = Socket(glm::ivec2(m_gridPosition.x + x, m_gridPosition.y + y));
                break;
            case Direction::WEST:
                m_output = Socket(glm::ivec2(m_gridPosition.x - x, m_gridPosition.y - y));
                break;
        }
    }
    
    void SetName() {
        m_name = "And Gate";
    }
        
};

#endif
