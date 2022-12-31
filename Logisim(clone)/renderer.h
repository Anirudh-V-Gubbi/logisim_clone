#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include <vector>
#include <memory>
#include <map>

class Renderer {
public:
    Renderer() {}
    ~Renderer() {}
    
    // add entities to the map - either to an existing list of entites with the same Texture ID or into a new list for a new Texture ID
    // --------------------------------------------------------------------
    void AddEntityToRender(Entity& entity) {
        
        auto it = m_entities.find(entity.GetTexID());
        // if Texture ID doesn't exist, insert a new pair and return its iterator
        // -------------------------------------------------------------
        if(it == m_entities.end()) {
            it = m_entities.insert(
                std::pair<GLuint, std::vector<std::shared_ptr<Entity>>>(
                     entity.GetTexID(),
                     std::vector<std::shared_ptr<Entity>>()
                )
          ).first;
        }
        
        // create a new shared ptr to own the instance of the entity and push it into the list
        // ------------------------------------------------------------------
        std::shared_ptr<Entity> ptr;
        ptr.reset(entity.GetInstance());
        it->second.push_back(ptr);
    }
    
    // draw the enitites - draw entities with same texture IDs together
    // ----------------------------------------------------------------
    void Draw(const glm::mat4& view, const glm::mat4& projection) const {
        for(const auto& [texID, entities] : m_entities) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texID);
            for(const auto& entity : entities) {
                entity->Draw(view, projection);
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
    
    
private:
    // A map of Texture Ids and Entitites, to bundle them and render them
    // under 1 texture bind call to increase performance
    // ------------------------------------------------------------------
    std::map<GLuint, std::vector<std::shared_ptr<Entity>>> m_entities;
};

#endif
