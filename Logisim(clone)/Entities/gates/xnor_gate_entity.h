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
    
    SocketState LogicFunction() override {
        return SocketState::UNINITIALIZED;
    }
    
    const char* GetName() const override {
        return "Xnor Gate";
    }
    
private:
    inline static GateFromScript *gate = NULL;
    
};

#endif
