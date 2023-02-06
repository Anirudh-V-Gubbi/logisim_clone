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
    
private:
    inline static GateFromScript *gate = NULL;
};

#endif


