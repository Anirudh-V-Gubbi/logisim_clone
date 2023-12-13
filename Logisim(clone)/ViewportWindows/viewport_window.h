#ifndef VIEWPORT_WINDOW_H
#define VIEWPORT_WINDOW_H

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include "../framebuffer.h"
#include "../renderer.h"
#include "../Shaders/shader.h"
#include "../Logger/logger.h"
#include "../Event/event.h"
#include "../Event/application_event.h"
#include "../Event/key_event.h"
#include "../Event/mouse_event.h"
#include "../Shaders/shader_manager.h"

#include <memory>

class ViewportWindow {
public:

    virtual ~ViewportWindow() {
        
    };
    
    virtual void Render(const glm::mat4& view, const glm::mat4& projection) const {
        // bind to the framebuffer and clear it
        // ------------------------------------
        m_frameBuffer->Bind();
        
        // set white background for the playground viewport
        // ------------------------------------------------
        if(m_position.x >= 0.19 && m_position.y >= 0.09)
            glClearColor(1.0, 1.0, 1.0, 1.0);
        else
            glClearColor(m_position.x * 5, m_windowDimensions.x, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // render the entities
        // -------------------
        m_renderer->Draw(view, projection);
        
        // bind to default framebuffer
        // ---------------------------
        m_frameBuffer->Unbind();
        
        // render the framebuffer rectangle
        // -------------------------------
        this->drawRectangle();
    }
    
    virtual void AddEntititesToViewport(std::shared_ptr<Entity> entity) {
        m_renderer->AddEntityToRender(entity);
    }
    
    virtual void HandleEvent(Event& event) = 0;
    
    glm::vec2 GetWindowDimensions() const {
        return this->m_windowDimensions;
    }
    
protected:
    glm::vec2 m_windowDimensions;
    glm::ivec2 m_screenDimensions;
    glm::vec2 m_position;
    std::unique_ptr<FrameBuffer> m_frameBuffer;
    std::unique_ptr<Renderer> m_renderer;
    std::shared_ptr<Shader> m_frameBufferShader;
    GLuint m_VBO, m_VAO, m_EBO;
    
    // Protected constructor for the abstract class
    // --------------------------------------------
    ViewportWindow(glm::vec2 fractionalWindowDimensions, glm::vec2 fractionalPosition,
                   const glm::ivec2& screenDimensions, std::shared_ptr<Shader> shader)
    : m_windowDimensions{fractionalWindowDimensions}, m_position{fractionalPosition},
    m_screenDimensions{screenDimensions}, m_frameBufferShader{shader} {
        // create a new framebuffer and a new renderer
        // -------------------------------------------
        m_frameBuffer = std::make_unique<FrameBuffer>(m_windowDimensions.x * m_screenDimensions.x, m_windowDimensions.y * m_screenDimensions.y);
        m_renderer = std::make_unique<Renderer>();
        
        // setup the array objects and buffers for the framebuffer rectangle
        // -----------------------------------------------------------------
        this->setup();
    }
    
    void drawRectangle() const {
        this->m_frameBufferShader->Use();
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
        float offsetX = 2.0f * m_windowDimensions.x;
        float offsetY = 2.0f * m_windowDimensions.y;
        
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
