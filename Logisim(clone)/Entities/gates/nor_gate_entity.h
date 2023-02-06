#ifndef NOR_GATE_ENTITY_H
#define NOR_GATE_ENTITY_H

#include "gate_entity.h"

class NorGateEntity : public GateEntity {
public:
    NorGateEntity(Shader& shader, glm::vec3 position, glm::ivec2 gridPosition)
    : GateEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        if(gate == NULL) {
            gate = parseScriptToGate("entity_nor_gate.txt");
            gate->texture.DeleteImageData();
        }
        GateEntity::InitializeTexture(gate->texture);
        GateEntity::InitializeInputs(*gate);
        GateEntity::InitializeOutput(*gate);
    }
    ~NorGateEntity() { }

    NorGateEntity* GetInstance() const override {
        return (NorGateEntity*)this;
    }
    
    SocketState LogicFunction() override {
        return SocketState::UNINITIALIZED;
    }
    
    const char* GetName() const override {
        return "Nor Gate";
    }
    
private:
    inline static GateFromScript *gate = NULL;
    
};

#endif
