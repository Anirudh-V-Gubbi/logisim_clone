#ifndef VIEWPORT_WINDOW_H
#define VIEWPORT_WINDOW_H

#include <glm/vec2.hpp>
#include "../framebuffer.h"
#include "../renderer.h"
#include "../Entities/entity.h"
#include "../Shaders/shader.h"

#include <memory>

class ViewportWindow {
public:
    ViewportWindow(glm::ivec2 windowDimensions, glm::vec2 fractionalPosition,
                   glm::ivec2 screenDimensions, Shader& shader)
    : m_windowDimensions{windowDimensions}, m_position{fractionalPosition},
    m_screenDimensions{screenDimensions}, m_frameBufferShader{shader} {
        // create a new framebuffer and a new renderer
        // -------------------------------------------
        m_frameBuffer = std::make_unique<FrameBuffer>(m_screenDimensions.x, m_screenDimensions.y);
        m_renderer = std::make_unique<Renderer>();
        
        // setup the array objects and buffers for the framebuffer rectangle
        // -----------------------------------------------------------------
        this->setup();
    }
    virtual ~ViewportWindow() { };
    
    virtual void Render() const {
        // bind to the framebuffer and clear it
        // ------------------------------------
        m_frameBuffer->Bind();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // render the entities
        // -------------------
        m_renderer->Draw();
        
        // bind to default framebuffer
        // ---------------------------
        m_frameBuffer->Unbind();
        
        // render the framebuffer rectangle
        // -------------------------------
        this->drawRectangle();
    }
    
    virtual void AddEntititesToViewport(Entity& entity) {
        m_renderer->AddEntityToRender(entity);
    }
    
private:
    glm::ivec2 m_windowDimensions;
    glm::ivec2 m_screenDimensions;
    glm::vec2 m_position;
    std::unique_ptr<FrameBuffer> m_frameBuffer;
    std::unique_ptr<Renderer> m_renderer;
    Shader m_frameBufferShader;
    GLuint m_VBO, m_VAO, m_EBO;
    
    void drawRectangle() const {
        this->m_frameBufferShader.Use();
        glBindVertexArray(this->m_VAO);
        this->m_frameBuffer->BindTextureAttachment();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    
    void setup() {
        // calculate top left position in [-1.0, 1.0] X [-1.0, 1.0] space
        // ---------------------------------------------------------------
        float normalizedPosX = m_position.x * 2.0f - 1.0f;
        float normalizedPosY = 1.0f - m_position.y * 2.0f;
        
        // calculate X and Y offsets by multiplying fractional width and height by 2.0f
        // due to the conversion fo coordinates from [0, 1] -> [-1, 1]
        // ----------------------------------------------------------------------------
        float offsetX = 2.0f * (float)m_windowDimensions.x / (float)m_screenDimensions.x;
        float offsetY = 2.0f * (float)m_windowDimensions.y / (float)m_screenDimensions.y;
        
        float vertices[] = {
            // positions   // texture coords
            normalizedPosX + offsetX,  normalizedPosY,            1.0f, 1.0f,   // top right
            normalizedPosX + offsetX,  normalizedPosY - offsetY,  1.0f, 0.0f,   // bottom right
            normalizedPosX,            normalizedPosY - offsetY,  0.0f, 0.0f,   // bottom left
            normalizedPosX,            normalizedPosY,            0.0f, 1.0f    // top left
        };
        unsigned int indices[] = {
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
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
    }
};

#endif
