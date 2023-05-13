#ifndef GLOBAL_GRID_H
#define GLOBAL_GRID_H

#include "entity.h"
#include "sockets/socket.h"
#include <glm/vec2.hpp>
#include <map>

class GlobalGrid : public Entity {
public:
    // grid requires absolute dimensions and position, relative to the window
    // ----------------------------------------------------------------------
    GlobalGrid(Shader& shader, Texture& texture, glm::vec3 position, glm::vec2 dimension)
    : m_dimension{dimension}, Entity(shader, texture, position) {
        instance = this;
        this->setup();
    }

    ~GlobalGrid() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        glDeleteBuffers(1, &m_instanceVBO);
        glDeleteProgram(m_shader.ID);
        
        Logger::GetInstance()->info("Deleted Global Grid");
    }
    
    GlobalGrid* GetInstance() const override {
        return (GlobalGrid*)this;
    }
    
    static GlobalGrid* GetGrid() {
        return instance;
    }
    
    void Draw(const glm::mat4& view, const glm::mat4& projection) const override {
        m_shader.Use();
        glBindVertexArray(m_VAO);
        m_shader.SetMatrix4f("view", view);
        m_shader.SetMatrix4f("projection", projection);
        m_shader.SetVector3f("gridColor", gridColor);
        
        // instance rendering for the grid dots
        // ------------------------------------
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, squaresCount.x * squaresCount.y);
        glBindVertexArray(0);
    }
    
    float GetSquareSpacing() const {
        return squareSpacing;
    }
    
    float GetSquareDimension() const {
        return squareDimension;
    }
    
    glm::ivec2 GetSquaresCount() const {
        return squaresCount;
    }
    
    glm::vec2 GetGridPointPosition(int i, int j) const {
        return (glm::vec2(m_position.x, m_position.y) + glm::vec2(j * squareSpacing, i * squareSpacing));
    }
    
    glm::vec2 GetGridPointPositionRelative(glm::vec2& origin, int i, int j) const {
        return (glm::vec2(origin.x, origin.y) + glm::vec2(j * squareSpacing, i * squareSpacing));
    }
    
    glm::vec2 GetGridPointPositionRelative(glm::vec3& origin, int i, int j) const {
        return (glm::vec2(origin.x, origin.y) + glm::vec2(j * squareSpacing, i * squareSpacing));
    }
    
    glm::ivec2 GetGridCoords(glm::vec2 position) const {
        
        float i = (position.y - m_position.y ) / squareSpacing;
        float j = (position.x - m_position.x ) / squareSpacing;
        
        int x = (int)i;
        int y = (int)j;
        if((float)i - (int)i > 0.7f) {
            x += 1;
        }
        if((float)j - (int)j > 0.5f) {
            y += 1;
        }
        
        return glm::ivec2(x, y);
    }
    
    void AddSocketToBoard(Socket& socket) {
        glm::ivec2 gridPos = socket.GetPosition();
        auto it = socketBoard.find(gridPos);
        
        if(it == socketBoard.end()) {
            it = socketBoard.insert({gridPos, std::vector<std::shared_ptr<Socket>>()}).first;
        }
        
        it->second.push_back(std::shared_ptr<Socket>(&socket));
    }
    
    void AddSocketsToBoard(std::vector<Socket>& sockets) {
        for(auto& socket : sockets) {
            AddSocketToBoard(socket);
        }
    }
    
private:
    inline static GlobalGrid* instance;
    unsigned int m_VBO, m_VAO, m_EBO, m_instanceVBO;
    glm::vec2 m_dimension;
    const float squareDimension = 1.0f;
    const float squareSpacing = 10.0f;
    glm::vec3 gridColor = glm::vec3(0.3);
    glm::ivec2 squaresCount;
    
    struct compare_ivec2{
        bool operator()(const glm::ivec2& first, const glm::ivec2& second) const {
            if(first.x == second.x) return first.y < second.y;
            
            return first.x < second.x;
        };
    };
    std::map<glm::ivec2, std::vector<std::shared_ptr<Socket>>, compare_ivec2> socketBoard;
    
    void setup() {
        // calculate the number of grid dots to be rendered in each axis
        // -------------------------------------------
        squaresCount.x = (m_dimension.x)/ (squareSpacing);
        squaresCount.y = (m_dimension.y)/ (squareSpacing - squareDimension);
                
        float vertices[] = {
            0.0f,  1.0f,
            0.0f,  0.0f,
            1.0f,  1.0f,
            1.0f,  0.0f,
        };
        
        int indices[] = {
            0, 1, 2,
            2, 1, 3
        };
        
        // calculate the positions of each dot
        // -----------------------------------
        float* translations = new float[2 * squaresCount.x * squaresCount.y];
        unsigned int squareIndex = 0;
        for(unsigned int i = 0; i < squaresCount.y; i++) {
            for(unsigned int j = 0; j < squaresCount.x; j++) {
                translations[squareIndex++] = j * squareSpacing;
                translations[squareIndex++] = i * squareSpacing;
            }
        }
        
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        glGenBuffers(1, &m_instanceVBO);
        glBindVertexArray(m_VAO);

        // buffer data and attrib pointers for vertex buffer
        // ---------------------------------------------------
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // buffer data and attrib pointers for instance buffer
        // ---------------------------------------------------
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, 2 * squaresCount.x * squaresCount.y * sizeof(float), translations, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        delete[] translations;
    }
};

#endif
