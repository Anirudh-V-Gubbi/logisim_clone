#ifndef ENTITY_H
#define ENTITY_H

#include <GL/glew.h>
#include "../Shaders/shader.h"
#include "../Textures/texture.h"

#include <iostream>

class Entity {
public:
    Shader m_shader;
    Texture m_texture;
    
    Entity(Shader& shader, Texture& texture) : m_shader{shader}, m_texture{texture} {}
    ~Entity() { }
    
    virtual void Draw() const = 0;
    
};

#endif
