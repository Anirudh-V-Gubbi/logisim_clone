#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

#include <GL/glew.h>
#include "stbImage.h"

class Texture {
public:
    GLuint ID = 0;
    
    Texture(const char* imagePath, bool persistentData = false) {
        specs = new ImageSpecs();
        this->generateTexture(imagePath);
        
        // delete image data and image specs if not required to be persistent
        // ------------------------------------------------------------------
        if(!persistentData) {
            if(specs->data) {
                stbi_image_free(specs->data);
            }
            
            delete specs;
            specs = nullptr;
        }
    }
    // empty texture
    // -------------
    Texture() {
        specs = NULL;
    }
    ~Texture() {
        
    }
    
    void Bind() {
        stbi_set_flip_vertically_on_load(true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
    
private:
    struct ImageSpecs {
        int width;
        int height;
        int nrChannels;
        unsigned char* data;
        
        ImageSpecs() : width{0}, height{0}, nrChannels{0}, data{nullptr} {}
    } *specs;
    
    void generateTexture(const char* path) {
        specs->data = stbi_load(path, &specs->width, &specs->height, &specs->nrChannels, 0);
        
        if(specs->data) {
            glGenTextures(1, &this->ID);
            glBindTexture(GL_TEXTURE_2D, this->ID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, specs->width, specs->height, 0, GL_RGB, GL_UNSIGNED_BYTE, specs->data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "ERROR | Failed to load texture" << std::endl;
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
