#ifndef NOT_GATE_ENTITY_H
#define NOT_GATE_ENTITY_H

#include "gate_entity.h"

class NotGateEntity : public GateEntity {
public:
    NotGateEntity(Shader& shader, glm::vec3 position, glm::ivec2 gridPosition)
    : GateEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        if(gate == NULL) {
            gate = parseScriptToGate("entity_not_gate.txt");
            gate->texture.DeleteImageData();
        }
        GateEntity::InitializeTexture(gate->texture);
        GateEntity::InitializeInputs(*gate);
        GateEntity::InitializeOutput(*gate);
    }
    ~NotGateEntity() { }
    
    NotGateEntity* GetInstance() const override {
        return (NotGateEntity*)this;
    }
    
    SocketState LogicFunction() override {
        return SocketState::UNINITIALIZED;
    }
    
    const char* GetName() const override {
        return "Not Gate";
    }
    
    void OnInputChange(SocketState newState) override {
        using ss = SocketState;
        ss finalState = ss::UNINITIALIZED;
        
        for(auto& socket : m_sockets.m_inputs) {
            ss state = socket.GetState();
            
            if(state == ss::UNINITIALIZED) {
                continue;
            }
            if(state == ss::ERROR) {
                finalState = ss::ERROR;
                break;
            }
            if(state == ss::LOW) {
                finalState = ss::HIGH;
            }
            else if(state == ss::HIGH) {
                finalState = ss::LOW;
            }
        }
        
        if(finalState == ss::UNINITIALIZED) {
            finalState = ss::ERROR;
        }
        
        if(m_sockets.m_outputs[0].GetState() != finalState) {
            m_sockets.m_outputs[0].ChangeState(finalState);
            GlobalGrid::GetGrid()->PushStateChangeNotification(m_sockets.m_outputs[0]);
        }
    }
    
private:
    inline static GateFromScript *gate = NULL;
};

#endif


