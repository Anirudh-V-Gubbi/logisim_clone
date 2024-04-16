#ifndef NOT_GATE_ENTITY_H
#define NOT_GATE_ENTITY_H

#include "gate_entity.h"

class NotGateEntity : public GateEntity {
public:
    NotGateEntity(std::shared_ptr<Shader> shader, glm::vec3 position, glm::ivec2 gridPosition)
    : GateEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        LOG_FUNCTION(this, shader, position, gridPosition);

        if(gate == NULL) {
            gate = parseScriptToGate("entity_not_gate.txt");
            gate->texture.DeleteImageData();
        }
        GateEntity::InitializeTexture(gate->texture);
        GateEntity::InitializeInputs(*gate);
        GateEntity::InitializeOutput(*gate);
    }
    ~NotGateEntity() {
        LOG_FUNCTION(this);
    }
    
    NotGateEntity* GetInstance() const override {
        LOG_FUNCTION(this);

        return (NotGateEntity*)this;
    }
    
    SocketState LogicFunction() const override {
        LOG_FUNCTION(this);

        using ss = SocketState;
        ss finalState = ss::UNINITIALIZED;
        
        for(auto& socket : m_sockets.m_inputs) {
            ss state = socket->GetState();
            
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
        
        return finalState;
    }
    
    const char* GetName() const override {
        LOG_FUNCTION(this);
        
        return "Not Gate";
    }
    
private:
    inline static GateFromScript *gate = NULL;
};

#endif


