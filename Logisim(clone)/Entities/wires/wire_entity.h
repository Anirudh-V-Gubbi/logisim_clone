#ifndef WIRE_ENTITY_H
#define WIRE_ENTITY_H

class WireEntity : public Entity {
public:
    inline static unsigned int VAO = 0, VBO = 0, EBO = 0;
    
    WireEntity(Shader& shader, glm::vec3 position) : Entity{shader, *EmptyTexture::GetInstance(), position} {
        
    }
    ~WireEntity() { }
    
    WireEntity* GetInstance() const override{
        return (WireEntity*)this;
    }
    
    void Draw(const glm::mat4& view, const glm::mat4& projection) const override {
        float* buffer = new float[2 * (m_sockets.size() - 1)];
        
        int i = 0;
        for(int j = 0; j < m_sockets.size() - 1; j++) {
            glm::vec2 pos = m_sockets[j].GetAbsPosition();
            buffer[i++] = pos.x;
            buffer[i++] = pos.y;
        }
        
        // buffer data and attrib pointers for instance buffer
        // ---------------------------------------------------
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, 2 * (m_sockets.size() - 1) * sizeof(float), NULL, GL_STATIC_DRAW);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 1);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * (m_sockets.size() - 1) * sizeof(float), buffer);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        m_shader.Use();
        
        m_shader.SetVector3f("size", glm::vec3(5.0f, 5.0f, 1.0f));
        m_shader.SetMatrix4f("view", view);
        m_shader.SetMatrix4f("projection", projection);
        
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (m_sockets.size() - 1));
        
        glBindVertexArray(0);
        
        delete[] buffer;
    }
    
private:
    unsigned int m_instanceVBO;
    std::vector<Socket> m_sockets;
    
    void setup() {
        if(VAO == 0 && EBO == 0 && VBO == 0) {
            // Vertex information and buffers setup
            // ------------------------------------
            
            //  +-------+
            //  |       | Origin is at the
            //  |X      | left center
            //  |       |
            //  +-------+
            float vertices[] = {
                 // positions
                 1.0f,    0.5f,   // top right
                 1.0f,   -0.5f,   // bottom right
                 0.0f,   -0.5f,   // bottom left
                 0.5f,    0.5f    // top left
            };
            unsigned int indices[] = {
                0, 3, 2,  // first Triangle
                0, 2, 1   // second Triangle
            };
            
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        
        glGenBuffers(1, &m_instanceVBO);
    }
    
};

#endif
