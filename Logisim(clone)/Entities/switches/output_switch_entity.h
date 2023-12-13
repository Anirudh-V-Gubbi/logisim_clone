#ifndef OUTPUT_SWITCH_ENTITY_H
#define OUTPUT_SWITCH_ENTITY_H

#include "switch_entity.h"

class OutputSwitchEntity : public SwitchEntity {
public:
    OutputSwitchEntity(std::shared_ptr<Shader> shader, glm::vec3 position, glm::ivec2 gridPosition)
    : SwitchEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        if(sswitch == NULL) {
            sswitch = parseScriptToSwitch("entity_output_switch.txt");
            sswitch->texture.DeleteImageData();
            sswitch->colorMapTexture.DeleteImageData();
        }
        m_switchState = SocketState::HIGH;
        SwitchEntity::InitializeSwitchEntity(*sswitch);
    }
    ~OutputSwitchEntity() { }

        OutputSwitchEntity* GetInstance() const override {
        return (OutputSwitchEntity*)this;
    }
    
    const char* GetName() const override {
        return "Output Switch";
    }
    
    void OnInputChange(SocketState newState) override {
        using ss = SocketState;
        ss finalState = ss::UNINITIALIZED;
        m_switchState = newState;
    }
    
private:
    inline static SwitchFromScript *sswitch = NULL;
    
};

#endif
