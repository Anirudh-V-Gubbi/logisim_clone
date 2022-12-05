#ifndef GLOBAL_GRID_H
#define GLOBAL_GRID_H

#include "entity.h"
#include <glm/vec2.hpp>

class GlobalGrid : public Entity {
public:
    // grid requires absolute dimensions and position, relative to the window
    // ----------------------------------------------------------------------
    GlobalGrid(Shader& shader, Texture& texture, glm::vec3 position, glm::vec2 dimension)
    : m_dimension{dimension}, Entity(shader, texture, position) {
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
    
    void Draw(const glm::mat4& view, const glm::mat4& projection) const override {
        m_shader.Use();
        glBindVertexArray(m_VAO);
        m_shader.SetMatrix4f("view", view);
        m_shader.SetMatrix4f("projection", projection);
        m_shader.SetVector3f("gridColor", gridColor);
        
        // instance rendering for the grid dots
        // ------------------------------------
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, squaresCount);
        glBindVertexArray(0);
    }
    
private:
    unsigned int m_VBO, m_VAO, m_EBO, m_instanceVBO;
    glm::vec2 m_dimension;
    const float squareDimension = 1.0f;
    const float squareSpacing = 10.0f;
    glm::vec3 gridColor = glm::vec3(0.3);
    unsigned int squaresCount;
    
    void setup() {
        // calculate the number of grid dots to be rendered in each axis
        // -------------------------------------------
        unsigned int xSquaresCount = (m_dimension.x)/ (squareSpacing);
        unsigned int ySquaresCount = (m_dimension.y)/ (squareSpacing - squareDimension);
        
        // total number of squares
        // -----------------------
        squaresCount = xSquaresCount * ySquaresCount;
        
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
        float* translations = new float[2 * squaresCount];
        unsigned int squareIndex = 0;
        for(unsigned int i = 0; i < ySquaresCount; i++) {
            for(unsigned int j = 0; j < xSquaresCount; j++) {
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
        glBufferData(GL_ARRAY_BUFFER, 2 * squaresCount * sizeof(float), translations, GL_STATIC_DRAW);
        
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
