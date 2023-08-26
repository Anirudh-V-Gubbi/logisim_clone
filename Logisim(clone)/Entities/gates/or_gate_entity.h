#ifndef OR_GATE_ENTITY_H
#define OR_GATE_ENTITY_H

#include "gate_entity.h"

class OrGateEntity : public GateEntity {
public:
    OrGateEntity(Shader& shader, glm::vec3 position, glm::ivec2 gridPosition)
    : GateEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        if(gate == NULL) {
            gate = parseScriptToGate("entity_or_gate.txt");
            gate->texture.DeleteImageData();
        }
        GateEntity::InitializeTexture(gate->texture);
        GateEntity::InitializeInputs(*gate);
        GateEntity::InitializeOutput(*gate);
    }
    ~OrGateEntity() { }

    OrGateEntity* GetInstance() const override {
        return (OrGateEntity*)this;
    }
    
    SocketState LogicFunction() const override {
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
            
            if(state == ss::HIGH) {
                finalState = ss::HIGH;
            }
            else if(state == ss::LOW) {
                if(finalState != ss::HIGH) {
                    finalState = ss::LOW;
                }
            }
        }
        
        if(finalState == ss::UNINITIALIZED) {
            finalState = ss::ERROR;
        }
        
        return finalState;
    }
    
    const char* GetName() const override {
        return "Or Gate";
    }
    
private:
    inline static GateFromScript *gate = NULL;
    
};

#endif
