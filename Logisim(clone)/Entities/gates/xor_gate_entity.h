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
    
    SocketState LogicFunction() override {
        return SocketState::UNINITIALIZED;
    }
    
    const char* GetName() const override {
        return "Xor Gate";
    }
    
private:
    inline static GateFromScript *gate = NULL;
    
};

#endif
