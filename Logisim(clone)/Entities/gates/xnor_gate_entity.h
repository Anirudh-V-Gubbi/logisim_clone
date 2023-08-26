#ifndef XNOR_GATE_ENTITY_H
#define XNOR_GATE_ENTITY_H

#include "gate_entity.h"

class XnorGateEntity : public GateEntity {
public:
    XnorGateEntity(Shader& shader, glm::vec3 position, glm::ivec2 gridPosition)
    : GateEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        if(gate == NULL) {
            gate = parseScriptToGate("entity_xnor_gate.txt");
            gate->texture.DeleteImageData();
        }
        GateEntity::InitializeTexture(gate->texture);
        GateEntity::InitializeInputs(*gate);
        GateEntity::InitializeOutput(*gate);
    }
    ~XnorGateEntity() { }

    XnorGateEntity* GetInstance() const override {
        return (XnorGateEntity*)this;
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
                if(finalState != ss::LOW) {
                    finalState = ss::HIGH;
                }
            }
            else if(state == ss::HIGH) {
                if(finalState == ss::LOW) {
                    finalState = ss::HIGH;
                }
                else {
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
        return "Xnor Gate";
    }
    
private:
    inline static GateFromScript *gate = NULL;
    
};

#endif
