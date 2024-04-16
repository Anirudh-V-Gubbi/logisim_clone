#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

#include <GL/glew.h>
#include "stbImage.h"
#include <utility.h>
#include <Logger/logger.h>

enum class TextureFormats {
    JPG,
    PNG
};

class Texture {
public:
    GLuint ID = 0;
    
    Texture(std::string imagePath, TextureFormats format, bool persistentData = false) {
        specs.reset(new ImageSpecs());
        m_textureName = imagePath.substr(imagePath.find('.'));
        this->generateTexture(getTextureFullPath(imagePath).c_str(), format);
        
        // delete image data and image specs if not required to be persistent
        // ------------------------------------------------------------------
        if(!persistentData) {
            if(specs->data) {
                stbi_image_free(specs->data);
                specs->data = nullptr;
            }
            
        }
    }
    // empty texture
    // -------------
    Texture() {
        specs = NULL;
    }
    ~Texture() { }
    
    void Bind() {
        stbi_set_flip_vertically_on_load(true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
    
    int GetTexWidth() const {
        if(specs != nullptr) {
            return specs->width;
        }
        
        return -1;
    }
    
    int GetTexHeight() const {
        if(specs != nullptr) {
            return specs->height;
        }
        
        return -1;
    }
    
    bool DeleteImageData()
    {
        if(specs != nullptr && specs->data != nullptr) {
            delete specs->data;
            specs->data = nullptr;
            return true;
        }
        
        return false;
    }
    
private:
    struct ImageSpecs {
        int width;
        int height;
        int nrChannels;
        unsigned char* data;
        
        ImageSpecs() : width{0}, height{0}, nrChannels{0}, data{nullptr} {}
    };
    std::shared_ptr<ImageSpecs> specs;
    std::string m_textureName;
    
    void generateTexture(std::string path, TextureFormats format) {
        specs->data = stbi_load(path.c_str(), &specs->width, &specs->height, &specs->nrChannels, format == TextureFormats::PNG ? STBI_rgb_alpha : STBI_default);
        
        if(specs->data) {
            glGenTextures(1, &this->ID);
            glBindTexture(GL_TEXTURE_2D, this->ID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            glTexImage2D(GL_TEXTURE_2D, 0, format == TextureFormats::PNG ? GL_RGBA : GL_RGB, specs->width, specs->height, 0, format == TextureFormats::PNG ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, specs->data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            Logger::GetInstance()->error("Failed to load texture", m_textureName);
        }
    }
};

// empty texture
// -------------
class EmptyTexture : public Texture {
public:
    ~EmptyTexture() { }
    
    static EmptyTexture* GetInstance() {
        if(s_instance == NULL) {
            s_instance = new EmptyTexture();
        }
        
        return s_instance;
    }
private:
    inline static EmptyTexture* s_instance = NULL;
    EmptyTexture() {}
    
};
#endif
