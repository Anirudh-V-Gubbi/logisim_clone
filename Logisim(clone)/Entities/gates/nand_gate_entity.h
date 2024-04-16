#ifndef NAND_GATE_ENTITY_H
#define NAND_GATE_ENTITY_H

#include "gate_entity.h"

class NandGateEntity : public GateEntity {
public:
    NandGateEntity(std::shared_ptr<Shader> shader, glm::vec3 position, glm::ivec2 gridPosition)
    : GateEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        LOG_FUNCTION(this, shader, position, gridPosition);

        if(gate == NULL) {
            gate = parseScriptToGate("entity_nand_gate.txt");
            gate->texture.DeleteImageData();
        }
        GateEntity::InitializeTexture(gate->texture);
        GateEntity::InitializeInputs(*gate);
        GateEntity::InitializeOutput(*gate);
    }
    ~NandGateEntity() {
        LOG_FUNCTION(this);
    }
    
    NandGateEntity* GetInstance() const override {
        LOG_FUNCTION(this);

        return (NandGateEntity*)this;
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
        LOG_FUNCTION(this);
        
        return "Nand Gate";
    }
    
private:
    inline static GateFromScript *gate = NULL;
};

#endif


