#ifndef XOR_GATE_ENTITY_H
#define XOR_GATE_ENTITY_H

#include "gate_entity.h"

class XorGateEntity : public GateEntity {
public:
    XorGateEntity(Shader& shader, glm::vec3 position, glm::ivec2 gridPosition)
    : GateEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        if(gate == NULL) {
            gate = parseScriptToGate("entity_xor_gate.txt");
            gate->texture.DeleteImageData();
        }
        GateEntity::InitializeTexture(gate->texture);
        GateEntity::InitializeInputs(*gate);
        GateEntity::InitializeOutput(*gate);
    }
    ~XorGateEntity() { }

    XorGateEntity* GetInstance() const override {
        return (XorGateEntity*)this;
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
            if(state == ss::LOW) {
                if(finalState != ss::HIGH) {
                    finalState = ss::LOW;
                }
            }
            else if(state == ss::HIGH) {
                if(finalState == ss::HIGH) {
                    finalState = ss::LOW;
                }
                else {
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
        return "Xor Gate";
    }
    
private:
    inline static GateFromScript *gate = NULL;
    
};

#endif
