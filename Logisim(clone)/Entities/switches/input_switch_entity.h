#ifndef INPUT_SWITCH_ENTITY_H
#define INPUT_SWITCH_ENTITY_H

#include "switch_entity.h"

class InputSwitchEntity : public SwitchEntity {
public:
    InputSwitchEntity(Shader& shader, glm::vec3 position, glm::ivec2 gridPosition)
    : SwitchEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        if(sswitch == NULL) {
            sswitch = parseScriptToSwitch("entity_input_switch.txt");
            sswitch->texture.DeleteImageData();
            sswitch->colorMapTexture.DeleteImageData();
        }
        m_switchState = SocketState::HIGH;
        SwitchEntity::InitializeSwitchEntity(*sswitch);
    }
    ~InputSwitchEntity() { }

        InputSwitchEntity* GetInstance() const override {
        return (InputSwitchEntity*)this;
    }
    
    const char* GetName() const override {
        return "Input Switch";
    }
    
private:
    inline static SwitchFromScript *sswitch = NULL;
    
};

#endif

