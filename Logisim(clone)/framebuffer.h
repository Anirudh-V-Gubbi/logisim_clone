#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <Logger/log.h>

class FrameBuffer {
public:
    FrameBuffer(unsigned int width, unsigned int height)
    : m_width{width}, m_height{height} {
        LOG_FUNCTION(this);

        // generate a framebuffer
        // ----------------------
        this->generateFramebuffer();
    }
    ~FrameBuffer() {
        LOG_FUNCTION(this);
    }
    
    void Bind() {
        LOG_FUNCTION(this);

        // bind to the new framebuffer
        // ---------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, this->m_FBO);
    }
    
    void Unbind() {
        LOG_FUNCTION(this);

        // bind to default framebuffer
        // ---------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    GLuint GetTextureAttachment() const {
        LOG_FUNCTION(this);

        // return texture ID of the texture attachment
        // -------------------------------------------
        return this->m_textureAttachment;
    }
    
    void BindTextureAttachment() const {
        LOG_FUNCTION(this);

        // bind the texture attachment
        // -------------------------------------------
        glBindTexture(GL_TEXTURE_2D, this->m_textureAttachment);
    }
    
private:
    GLuint m_FBO;
    GLuint m_textureAttachment;
    unsigned int m_width, m_height;
    
    void generateFramebuffer() {
        LOG_FUNCTION(this);
        
        glGenFramebuffers(1, &this->m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, this->m_FBO);
        
        // generate a texture attcahment
        // -----------------------------
        glGenTextures(1, &m_textureAttachment);
        glBindTexture(GL_TEXTURE_2D, m_textureAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->m_width, this->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // attach it to currently bound framebuffer object
        // -----------------------------------------------
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureAttachment, 0);
        
        // framebuffer generation unsuccessfull
        // ------------------------------------
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Logger::GetInstance()->error("Framebuffer is not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};

#endif
