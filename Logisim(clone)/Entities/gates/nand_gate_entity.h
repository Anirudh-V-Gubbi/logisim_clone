#ifndef NAND_GATE_ENTITY_H
#define NAND_GATE_ENTITY_H

#include "gate_entity.h"

class NandGateEntity : public GateEntity {
public:
    NandGateEntity(Shader& shader, glm::vec3 position, glm::ivec2 gridPosition)
    : GateEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        if(gate == NULL) {
            gate = parseScriptToGate("entity_nand_gate.txt");
            gate->texture.DeleteImageData();
        }
        GateEntity::InitializeTexture(gate->texture);
        GateEntity::InitializeInputs(*gate);
        GateEntity::InitializeOutput(*gate);
    }
    ~NandGateEntity() { }
    
    NandGateEntity* GetInstance() const override {
        return (NandGateEntity*)this;
    }
    
    SocketState LogicFunction() override {
        return SocketState::UNINITIALIZED;
    }
    
    const char* GetName() const override {
        return "Nand Gate";
    }
    
private:
    inline static GateFromScript *gate = NULL;
};

#endif


