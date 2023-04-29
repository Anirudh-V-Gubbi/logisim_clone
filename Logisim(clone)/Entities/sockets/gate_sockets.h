#ifndef GATE_SOCKETS_H
#define GATE_SOCKETS_H

#include "socket.h"

class GateSockets {
public:
    inline static unsigned int m_VBO = 0, m_VAO = 0, m_EBO = 0;
    unsigned int m_instanceVBO = 0;
    inline static Shader* shader = nullptr;
    inline static Texture* texture = nullptr;
    
    std::vector<Socket> m_inputs;
    std::vector<Socket> m_outputs;
    
    GateSockets() {
        this->setup();
    }
    ~GateSockets() {
        glDeleteBuffers(1, &m_instanceVBO);
    }
    
    void Draw(const glm::mat4& view, const glm::mat4& projection) const {
        
        float* buffer = new float[5 * (m_inputs.size() + m_outputs.size())];
        
        int i = 0;
        for(auto& socket : m_inputs) {
            auto position = socket.GetAbsPosition();
            auto color = socket.GetColor();
            buffer[i++] = position.x;
            buffer[i++] = position.y;
            buffer[i++] = color.x;
            buffer[i++] = color.y;
            buffer[i++] = color.z;
        }
        
        for(auto& socket : m_outputs) {
            auto position = socket.GetAbsPosition();
            auto color = socket.GetColor();
            buffer[i++] = position.x;
            buffer[i++] = position.y;
            buffer[i++] = color.x;
            buffer[i++] = color.y;
            buffer[i++] = color.z;
        }
        
        // buffer data and attrib pointers for instance buffer
        // ---------------------------------------------------
        glBindVertexArray(m_VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, 5 * (m_inputs.size() + m_outputs.size()) * sizeof(float), NULL, GL_STATIC_DRAW);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, 5 * (m_inputs.size() + m_outputs.size()) * sizeof(float), buffer);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        shader->Use();
        texture->Bind();
        
        shader->SetVector3f("size", glm::vec3(texture->GetTexWidth(), texture->GetTexHeight(), 1.0f));
        shader->SetMatrix4f("view", view);
        shader->SetMatrix4f("projection", projection);
        
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_inputs.size() + m_outputs.size());
        
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        
        delete[] buffer;
    }
    
private:
    void setup() {
        if(m_VBO == 0 && m_VAO == 0 && m_EBO == 0) {
            
            // Vertex information and buffers setup
            // ------------------------------------
            
            //  +-------+
            //  |       | Origin is at the
            //  |       | bottom center
            //  |   x   |
            //  +-------+
            float vertices[] = {
                 // positions // texture coords
                  0.5f,   1.0f,  1.0f, 1.0f,   // top right
                  0.5f,   0.0f,  1.0f, 0.0f,   // bottom right
                 -0.5f,   0.0f,  0.0f, 0.0f,   // bottom left
                 -0.5f,   1.0f,  0.0f, 1.0f    // top left
            };
            unsigned int indices[] = {
                0, 3, 2,  // first Triangle
                0, 2, 1   // second Triangle
            };
            
            glGenVertexArrays(1, &m_VAO);
            glGenBuffers(1, &m_VBO);
            glGenBuffers(1, &m_EBO);
            glBindVertexArray(m_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            
            // shader setup
            // ------------
            if(shader == nullptr) {
                shader = new Shader("resources/shaders/socket_vertex.vs", "resources/shaders/socket_fragment.fs");
            }
            
            // texture setup
            // -------------
            if(texture == nullptr) {
                texture = new Texture(".//resources/images/socket.png", TextureFormats::PNG);
            }
        }
        
        glGenBuffers(1, &m_instanceVBO);
    }
};

#endif
