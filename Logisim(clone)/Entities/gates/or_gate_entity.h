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
    
    SocketState LogicFunction() override {
        return SocketState::UNINITIALIZED;
    }
    
    const char* GetName() const override {
        return "Or Gate";
    }
    
private:
    inline static GateFromScript *gate = NULL;
    
};

#endif
