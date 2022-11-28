#ifndef VIEWPORT_WINDOW_H
#define VIEWPORT_WINDOW_H

#include "../framebuffer.h"
#include "../renderer.h"
#include "../Entities/entity.h"
#include "../Shaders/shader.h"

#include <memory>

class ViewportWindow {
public:
    ViewportWindow(unsigned int width, unsigned int height, Shader& shader)
    : m_width{width}, m_height{height}, m_frameBufferShader{shader} {
        // create a new framebuffer and a new renderer
        // -------------------------------------------
        m_frameBuffer = std::make_unique<FrameBuffer>(width, height);
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
    unsigned int m_width, m_height;
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
        float vertices[] = {
            // positions   // texture coords
             2.0f,  1.0f,  1.0f, 1.0f,   // top right
             2.0f, -1.0f,  1.0f, 0.0f,   // bottom right
             0.0f, -1.0f,  0.0f, 0.0f,   // bottom left
             0.0f,  1.0f,  0.0f, 1.0f    // top left
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
