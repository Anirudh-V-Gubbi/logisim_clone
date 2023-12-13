#ifndef AND_GATE_ENTITY_H
#define AND_GATE_ENTITY_H

#include "gate_entity.h"

class AndGateEntity : public GateEntity {
public:
    AndGateEntity(std::shared_ptr<Shader> shader, glm::vec3 position, glm::ivec2 gridPosition)
    : GateEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        if(gate == NULL) {
            gate = parseScriptToGate("entity_and_gate.txt");
            gate->texture.DeleteImageData();
        }
        GateEntity::InitializeTexture(gate->texture);
        GateEntity::InitializeInputs(*gate);
        GateEntity::InitializeOutput(*gate);
    }
    ~AndGateEntity() { }

    AndGateEntity* GetInstance() const override {
        return (AndGateEntity*)this;
    }
    
    SocketState LogicFunction() const override {
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
                finalState = ss::LOW;
            }
            else if(state == ss::HIGH) {
                if(finalState != ss::LOW) {
                    finalState = ss::HIGH;
                }
            }
        }
        
        if(finalState == ss::UNINITIALIZED) {
            finalState = ss::ERROR;
        }
        
        return finalState;
    }
    
    const char* GetName() const override {
        return "And Gate";
    }
    
private:
    inline static GateFromScript *gate = NULL;
    
};

#endif
