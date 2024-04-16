#ifndef ENTITY_H
#define ENTITY_H

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/io.hpp>
#include <Shaders/shader.h>
#include <Textures/texture.h>
#include <Logger/log.h>

#include <iostream>
#include <memory>

class Entity {
public:
    
    virtual ~Entity() {
        LOG_FUNCTION(this);
    }
    virtual Entity* GetInstance() const = 0;
    
    virtual void Draw(const glm::mat4& view, const glm::mat4& projection) const = 0;
    
    glm::vec3 GetPosition() const {
        LOG_FUNCTION(this);

        return m_position;
    }
    
    GLuint GetTexID() const {
        LOG_FUNCTION(this);

        return m_texture.ID;
    }
    
protected:
    std::shared_ptr<Shader> m_shader;
    Texture m_texture;
    glm::vec3 m_position;
    
    // Protected constructor for the abstract class
    // --------------------------------------------
    Entity(std::shared_ptr<Shader> shader, Texture& texture, glm::vec3 position) : m_shader{shader}, m_texture{texture}, m_position{position} {
        LOG_FUNCTION(this, shader, texture, position);
    }
};

#endif
