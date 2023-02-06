#ifndef AND_GATE_ENTITY_H
#define AND_GATE_ENTITY_H

#include "gate_entity.h"

class AndGateEntity : public GateEntity {
public:
    AndGateEntity(Shader& shader, glm::vec3 position, glm::ivec2 gridPosition)
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
    
    SocketState LogicFunction() override {
        return SocketState::UNINITIALIZED;
    }
    
    const char* GetName() const override {
        return "And Gate";
    }
    
private:
    inline static GateFromScript *gate = NULL;
    
};

#endif
