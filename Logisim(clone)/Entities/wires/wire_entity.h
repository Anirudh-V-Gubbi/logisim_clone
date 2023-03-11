#ifndef WIRE_ENTITY_H
#define WIRE_ENTITY_H

const float WIRE_HEIGHT = 4.0f;
const float PI2 = 3.1415f / 2.0f;
const float PI = 3.1415f;

class WireEntity : public Entity {
public:
    inline static unsigned int VAO = 0, VBO = 0, EBO = 0;
    
    WireEntity(Shader& shader, glm::vec3 position) : Entity{shader, *EmptyTexture::GetInstance(), position} {
        this->setup();
    }
    ~WireEntity() {
        glDeleteBuffers(1, &m_instanceVBO);
    }
    
    WireEntity* GetInstance() const override{
        return (WireEntity*)this;
    }
    
    Socket GetLastSocket() const{
        return m_sockets.back();
    }
    
    void CreateWireFromPoints(glm::ivec2 gridPoint1, glm::vec2 absPosition1, glm::ivec2 gridPoint2, glm::vec2 absPosition2) {
        glm::ivec2 offset = gridPoint2 - gridPoint1;
        glm::vec2 intersection = glm::vec2(absPosition2.x, absPosition1.y);
        
        int i;
        
        short int sgnY = (offset.y >= 0) - (offset.y < 0);
        float rotation = (1 - sgnY) * PI2;
        for(i = 0; abs(i) < abs(offset.y); i += sgnY) {
            AddSocket(Socket(glm::ivec2(gridPoint1.x, gridPoint1.y + i), GlobalGrid::GetGrid()->GetGridPointPositionRelative(absPosition1, 0, i)));
        }
        
        if(offset.y != 0) {
            AddSocket(Socket(glm::ivec2(gridPoint1.x, gridPoint1.y + i), GlobalGrid::GetGrid()->GetGridPointPositionRelative(absPosition1, 0, i)));
        }
        
        short int sgnX = (offset.x >= 0) - (offset.x < 0);
        rotation = (2 + sgnX) * PI2;
        for(i = 0; abs(i) < abs(offset.x); i += sgnX) {
            AddSocket(Socket(glm::ivec2(gridPoint1.x + i, gridPoint2.y), GlobalGrid::GetGrid()->GetGridPointPositionRelative(intersection, i, 0)));
        }
        
        if(offset.x != 0) {
            AddSocket(Socket(glm::ivec2(gridPoint1.x + i, gridPoint2.y), GlobalGrid::GetGrid()->GetGridPointPositionRelative(intersection, i, 0)));
        }
        
    }
    
    void ContinueBuildWire(glm::ivec2 gridPoint2, glm::vec2 absPosition2) {
        glm::ivec2 gridPoint1 = m_sockets.back().GetPosition();
        glm::vec2 absPosition1 = m_sockets.back().GetAbsPosition();
        
        glm::ivec2 offset = gridPoint2 - gridPoint1;;
        glm::vec2 intersection = glm::vec2(absPosition2.x, absPosition1.y);
        
        int i;
        
        short int sgnY = (offset.y >= 0) - (offset.y < 0);
        float rotation = (1 - sgnY) * PI2;
        for(i = 1 * sgnY; abs(i) < abs(offset.y); i += sgnY) {
            AddSocket(Socket(glm::ivec2(gridPoint1.x, gridPoint1.y + i), GlobalGrid::GetGrid()->GetGridPointPositionRelative(absPosition1, 0, i)));
        }
        
        if(offset.y != 0) {
            AddSocket(Socket(glm::ivec2(gridPoint1.x, gridPoint1.y + i), GlobalGrid::GetGrid()->GetGridPointPositionRelative(absPosition1, 0, i)));
        }
        
        short int sgnX = (offset.x >= 0) - (offset.x < 0);
        rotation = (2 + sgnX) * PI2;
        for(i = 1 * sgnX; abs(i) < abs(offset.x); i += sgnX) {
            AddSocket(Socket(glm::ivec2(gridPoint1.x + i, gridPoint2.y), GlobalGrid::GetGrid()->GetGridPointPositionRelative(intersection, i, 0)));
        }
        
        if(offset.x != 0) {
            AddSocket(Socket(glm::ivec2(gridPoint1.x + i, gridPoint2.y), GlobalGrid::GetGrid()->GetGridPointPositionRelative(intersection, i, 0)));
        }
        
    }
    
    void Draw(const glm::mat4& view, const glm::mat4& projection) const override {
        float* buffer = new float[3 * (m_sockets.size() - 1)];
        
        int i = 0;
        for(int j = 0; j < m_sockets.size() - 1; j++) {
            glm::vec2 pos = m_sockets[j].GetAbsPosition();
            buffer[i++] = pos.x;
            buffer[i++] = pos.y + WIRE_HEIGHT / 2;
            buffer[i++] = m_socketRotations[j];
        }
        
        // buffer data and attrib pointers for instance buffer
        // ---------------------------------------------------
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, 3 * (m_sockets.size() - 1) * sizeof(float), NULL, GL_STATIC_DRAW);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 1);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);
                
        glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * (m_sockets.size() - 1) * sizeof(float), buffer);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        m_shader.Use();
        
        m_shader.SetVector2f("size", glm::vec2(GlobalGrid::GetGrid()->GetSquareSpacing() + GlobalGrid::GetGrid()->GetSquareDimension(), WIRE_HEIGHT));
        m_shader.SetMatrix4f("view", view);
        m_shader.SetMatrix4f("projection", projection);
        m_shader.SetVector3f("wcolor", m_sockets[0].GetColor());
        
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (m_sockets.size() - 1));
        
        glBindVertexArray(0);
        
        delete[] buffer;
    }
    
    void AddSocket(Socket socket) {
        if(m_sockets.size() >= 1) {
            Socket& lastSocket = m_sockets.back();
            
            if(socket.GetPosition().x == lastSocket.GetPosition().x) {
                if(socket.GetPosition().y > lastSocket.GetPosition().y) {
                    m_socketRotations.push_back(0.0f);
                }
                else {
                    m_socketRotations.push_back(PI);
                }
            }
            else {
                if(socket.GetPosition().x > lastSocket.GetPosition().x) {
                    m_socketRotations.push_back(3 * PI2);
                }
                else {
                    m_socketRotations.push_back(PI2);
                }
            }
        }
        
        m_sockets.push_back(socket);
    }
    
private:
    unsigned int m_instanceVBO;
    std::vector<Socket> m_sockets;
    std::vector<float> m_socketRotations;
    
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
                 1.0f,    -0.5f,   // bottom right
                 0.0f,    -0.5f,   // bottom left
                 0.0f,    0.5f    // top left
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
