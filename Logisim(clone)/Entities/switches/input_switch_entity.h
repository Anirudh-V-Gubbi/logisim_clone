#ifndef INPUT_SWITCH_ENTITY_H
#define INPUT_SWITCH_ENTITY_H

#include "switch_entity.h"

class InputSwitchEntity : public SwitchEntity {
public:
    InputSwitchEntity(std::shared_ptr<Shader> shader, glm::vec3 position, glm::ivec2 gridPosition)
    : SwitchEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        LOG_FUNCTION(this, shader, position, gridPosition);

        if(sswitch == NULL) {
            sswitch = parseScriptToSwitch("entity_input_switch.txt");
            sswitch->texture.DeleteImageData();
            sswitch->colorMapTexture.DeleteImageData();
        }
        m_switchState = SocketState::HIGH;
        SwitchEntity::InitializeSwitchEntity(*sswitch);
    }
    ~InputSwitchEntity() {
        LOG_FUNCTION(this);
    }

    InputSwitchEntity* GetInstance() const override {
        LOG_FUNCTION(this);

        return (InputSwitchEntity*)this;
    }
    
    const char* GetName() const override {
        LOG_FUNCTION(this);
        
        return "Input Switch";
    }
    
private:
    inline static SwitchFromScript *sswitch = NULL;
    
};

#endif

