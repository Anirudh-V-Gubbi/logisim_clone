#ifndef OUTPUT_SWITCH_ENTITY_H
#define OUT PUT_SWITCH_ENTITY_H

#include "switch_entity.h"

class OutputSwitchEntity : public SwitchEntity {
public:
    OutputSwitchEntity(Shader& shader, glm::vec3 position, glm::ivec2 gridPosition)
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
    
private:
    inline static SwitchFromScript *sswitch = NULL;
    
};

#endif
