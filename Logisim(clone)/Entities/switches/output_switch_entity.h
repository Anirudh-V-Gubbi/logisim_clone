#ifndef OUTPUT_SWITCH_ENTITY_H
#define OUTPUT_SWITCH_ENTITY_H

#include "switch_entity.h"

class OutputSwitchEntity : public SwitchEntity {
public:
    OutputSwitchEntity(std::shared_ptr<Shader> shader, glm::vec3 position, glm::ivec2 gridPosition)
    : SwitchEntity(shader, *EmptyTexture::GetInstance(), position, gridPosition) {
        LOG_FUNCTION(this, shader, position, gridPosition);
        
        if(sswitch == NULL) {
            sswitch = parseScriptToSwitch("entity_output_switch.txt");
            sswitch->texture.DeleteImageData();
            sswitch->colorMapTexture.DeleteImageData();
        }
        m_switchState = SocketState::HIGH;
        SwitchEntity::InitializeSwitchEntity(*sswitch);
    }
    ~OutputSwitchEntity() {
        LOG_FUNCTION(this);
    }

    OutputSwitchEntity* GetInstance() const override {
        LOG_FUNCTION(this);

        return (OutputSwitchEntity*)this;
    }
    
    const char* GetName() const override {
        LOG_FUNCTION(this);

        return "Output Switch";
    }
    
    void OnInputChange(SocketState newState) override {
        LOG_FUNCTION(this, newState);

        using ss = SocketState;
        ss finalState = ss::UNINITIALIZED;
        m_switchState = newState;
    }
    
private:
    inline static SwitchFromScript *sswitch = NULL;
    
};

#endif
