#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#include <vector>
#include <map>

class Renderer {
public:
    std::map<GLuint, std::vector<Entity*>> m_entities;
    
    static Renderer* GetRenderer() {
        if(m_renderer == nullptr)
            m_renderer = new Renderer();
        
        return m_renderer;
    }
    ~Renderer() {}
    
    void AddEntityToRender(Entity& entity) {
        
        auto it = m_entities.find(entity.m_texture.ID);
        if(it == m_entities.end()) {
            it = m_entities.insert(
                std::pair<GLuint, std::vector<Entity*>>(
                     entity.m_texture.ID,
                     std::vector<Entity*>()
                )
          ).first;
        }
        
        it->second.push_back(&entity);
    }
    
    void Draw() const {
        for(const auto& [texID, entities] : m_entities) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texID);
            for(const auto entity : entities) {
                entity->Draw();
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
private:
    static inline Renderer* m_renderer = nullptr;
    
    Renderer() {}
};

#endif
