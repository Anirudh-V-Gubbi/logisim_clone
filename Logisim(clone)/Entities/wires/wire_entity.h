#ifndef WIRE_ENTITY_H
#define WIRE_ENTITY_H

#include "doubly_linked_list.h"
#include <Logger/log.h>

const float WIRE_HEIGHT = 4.0f;
const float PI2 = 3.1415f / 2.0f;
const float PI = 3.1415f;

class WireEntity : public Entity {
public:
    inline static unsigned int VAO = 0, VBO = 0, EBO = 0;
    
    WireEntity(std::shared_ptr<Shader> shader, glm::vec3 position) : Entity{shader, *EmptyTexture::GetInstance(), position} {
        LOG_FUNCTION(this, shader, position);
        this->setup();
        m_onInputChange = [this](SocketState state) {this->OnInputChange(state);};
    }
    ~WireEntity() {
        LOG_FUNCTION(this);

        glDeleteBuffers(1, &m_instanceVBO);
    }
    
    WireEntity* GetInstance() const override{
        LOG_FUNCTION(this);
        
        return (WireEntity*)this;
    }
    
    Socket& GetLastSocket() const{
        LOG_FUNCTION(this);

        return *m_sockets.back().get();
    }
    
    void CreateWireFromPoints(glm::ivec2 gridPoint1, glm::vec2 absPosition1, glm::ivec2 gridPoint2, glm::vec2 absPosition2) {
        LOG_FUNCTION(this, gridPoint1, absPosition1, gridPoint2, absPosition2);
        
        glm::ivec2 offset = gridPoint2 - gridPoint1;
        glm::vec2 intersection = glm::vec2(absPosition2.x, absPosition1.y);
        
        int i;
        
        short int sgnY = (offset.y >= 0) - (offset.y < 0);
        float rotation = (1 - sgnY) * PI2;
        for(i = 0; abs(i) < abs(offset.y); i += sgnY) {
            AddSocket(std::make_shared<Socket>(glm::ivec2(gridPoint1.x, gridPoint1.y + i), GlobalGrid::GetGrid()->GetGridPointPositionRelative(absPosition1, 0, i)));
        }
        
        if(offset.y != 0) {
            AddSocket(std::make_shared<Socket>(glm::ivec2(gridPoint1.x, gridPoint1.y + i), GlobalGrid::GetGrid()->GetGridPointPositionRelative(absPosition1, 0, i)));
        }
        
        short int sgnX = (offset.x >= 0) - (offset.x < 0);
        rotation = (2 + sgnX) * PI2;
        for(i = 0; abs(i) < abs(offset.x); i += sgnX) {
            AddSocket(std::make_shared<Socket>(glm::ivec2(gridPoint1.x + i, gridPoint2.y), GlobalGrid::GetGrid()->GetGridPointPositionRelative(intersection, i, 0)));
        }
        
        if(offset.x != 0) {
            AddSocket(std::make_shared<Socket>(glm::ivec2(gridPoint1.x + i, gridPoint2.y), GlobalGrid::GetGrid()->GetGridPointPositionRelative(intersection, i, 0)));
        }
        
    }
    
    void ContinueBuildWire(glm::ivec2 gridPoint2, glm::vec2 absPosition2) {
        LOG_FUNCTION(this, gridPoint2, absPosition2);
        
        glm::ivec2 gridPoint1 = m_sockets.back()->GetPosition();
        glm::vec2 absPosition1 = m_sockets.back()->GetAbsPosition();
        
        glm::ivec2 offset = gridPoint2 - gridPoint1;;
        glm::vec2 intersection = glm::vec2(absPosition2.x, absPosition1.y);
        
        int i;
        
        short int sgnY = (offset.y >= 0) - (offset.y < 0);
        float rotation = (1 - sgnY) * PI2;
        for(i = 1 * sgnY; abs(i) < abs(offset.y); i += sgnY) {
            AddSocket(std::make_shared<Socket>(glm::ivec2(gridPoint1.x, gridPoint1.y + i), GlobalGrid::GetGrid()->GetGridPointPositionRelative(absPosition1, 0, i)));
        }
        
        if(offset.y != 0) {
            AddSocket(std::make_shared<Socket>(glm::ivec2(gridPoint1.x, gridPoint1.y + i), GlobalGrid::GetGrid()->GetGridPointPositionRelative(absPosition1, 0, i)));
        }
        
        short int sgnX = (offset.x >= 0) - (offset.x < 0);
        rotation = (2 + sgnX) * PI2;
        for(i = 1 * sgnX; abs(i) < abs(offset.x); i += sgnX) {
            AddSocket(std::make_shared<Socket>(glm::ivec2(gridPoint1.x + i, gridPoint2.y), GlobalGrid::GetGrid()->GetGridPointPositionRelative(intersection, i, 0)));
        }
        
        if(offset.x != 0) {
            AddSocket(std::make_shared<Socket>(glm::ivec2(gridPoint1.x + i, gridPoint2.y), GlobalGrid::GetGrid()->GetGridPointPositionRelative(intersection, i, 0)));
        }
        
    }
    
    void Draw(const glm::mat4& view, const glm::mat4& projection) const override {
        LOG_FUNCTION(this);
        
        float* buffer = new float[3 * (m_sockets.size() - 1)];
        
        int i = 0, j = 0;
        for(auto& socket : m_sockets) {
            glm::vec2 pos = socket->GetAbsPosition();
            buffer[i++] = pos.x;
            buffer[i++] = pos.y + WIRE_HEIGHT / 2;
            buffer[i++] = m_socketRotations[j];
            
            if(++j == m_sockets.size() - 1) {
                break;
            }
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
        
        m_shader->Use();
        
        m_shader->SetVector2f("size", glm::vec2(GlobalGrid::GetGrid()->GetSquareSpacing() + GlobalGrid::GetGrid()->GetSquareDimension(), WIRE_HEIGHT));
        m_shader->SetMatrix4f("view", view);
        m_shader->SetMatrix4f("projection", projection);
        m_shader->SetVector3f("wcolor", m_sockets.front()->GetColor());
        
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (m_sockets.size() - 1));
        
        glBindVertexArray(0);
        
        delete[] buffer;
    }
    
    void AddSocket(std::shared_ptr<Socket> socket) {
        LOG_FUNCTION(this, socket);

        if(m_sockets.size() >= 1) {
            Socket& lastSocket = *m_sockets.back().get();
            
            if(socket->GetPosition().x == lastSocket.GetPosition().x) {
                if(socket->GetPosition().y > lastSocket.GetPosition().y) {
                    m_socketRotations.push_back(0.0f);
                }
                else {
                    m_socketRotations.push_back(PI);
                }
            }
            else {
                if(socket->GetPosition().x > lastSocket.GetPosition().x) {
                    m_socketRotations.push_back(3 * PI2);
                }
                else {
                    m_socketRotations.push_back(PI2);
                }
            }
        }
        
        socket->RegisterChangeCallback(&m_onInputChange);
        m_sockets.push_back(socket);
        GlobalGrid::GetGrid()->AddSocketToBoard(m_sockets.back());
    }
    
    void OnInputChange(SocketState newState) {
        LOG_FUNCTION(this, newState);

        for(auto& socket : m_sockets) {
            if(socket->GetState() == newState) continue;
            
            socket->ChangeState(newState, false);
            GlobalGrid::GetGrid()->PushStateChangeNotification(*socket.get());
        }
    }
    
private:
    unsigned int m_instanceVBO;
    DoublyLinkedList<std::shared_ptr<Socket>> m_sockets;
    std::vector<float> m_socketRotations;
    std::function<void(SocketState)> m_onInputChange;
    
    void setup() {
        LOG_FUNCTION(this);
        
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
