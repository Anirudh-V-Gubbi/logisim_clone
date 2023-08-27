#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Logger/logger.h"
#include "../utility.h"

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath) {
        // create a struct to hold shader code, deallocated in compileShaders()
        shaderCode = new ShaderCodes();
        
        this->readFiles(getShaderFullPath(vertexPath).c_str(), getShaderFullPath(fragmentPath).c_str());
        this->compileShaders();
    }
    
    ~Shader() {
        Logger::GetInstance()->info("Destroyed Shader");
        glDeleteProgram(this->ID);
    }
    
    void Use() const {
        glUseProgram(this->ID);
    };
    
    void SetMatrix4f(const char* name, glm::mat4 matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    void SetVector2f(const char* name, glm::vec2 vector) const {
        glUniform2fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(vector));
    }
    
    void SetVector3f(const char* name, glm::vec3 vector) const {
        glUniform3fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(vector));
    }
    
    void SetInteger1i(const char* name, int unit) const {
        glUniform1i(glGetUniformLocation(this->ID, name), unit);
    }
    
private:

    struct ShaderCodes {
        std::string vertexCode;
        std::string fragmentCode;
    } *shaderCode;
    
    void readFiles(const char* vertexPath, const char* fragPath){
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragPath);
            
            std::stringstream sCodeBuffer, fCodeBuffer;
            
            sCodeBuffer << vShaderFile.rdbuf();
            fCodeBuffer << fShaderFile.rdbuf();
            
            this->shaderCode->vertexCode = sCodeBuffer.str();
            this->shaderCode->fragmentCode = fCodeBuffer.str();
            
            vShaderFile.close();
            fShaderFile.close();
        }
        catch(std::ifstream::failure e) {
            std::cout << "ERROR | Failed to read shader files.\n";
        }
    }
    
    void compileShaders() {
        GLuint vertexID, fragID;
        
        const char* vertexCode = shaderCode->vertexCode.c_str();
        const char* fragmentCode = shaderCode->fragmentCode.c_str();
        
        //create vertex shader
        vertexID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexID, 1, &vertexCode, NULL);
        glCompileShader(vertexID);
        
        int success;
        char infolog[512];
        glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
        
        //vertex shader compilation failed
        if(!success) {
            glGetShaderInfoLog(vertexID, 512, NULL, infolog);
            std::cout << "ERROR | Failed to compile vertex shader : " << infolog << std::endl;
        }
        
        //create fragment shader
        fragID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragID, 1, &fragmentCode, NULL);
        glCompileShader(fragID);
        
        glGetShaderiv(fragID, GL_COMPILE_STATUS, &success);
        
        //fragment shader compilation failed
        if(!success) {
            glGetShaderInfoLog(fragID, 512, NULL, infolog);
            std::cout << "ERROR | Failed to compile fragment shader : " << infolog << std::endl;
        }
        
        //create and link shader program
        this->ID = glCreateProgram();
        glAttachShader(this->ID, vertexID);
        glAttachShader(this->ID, fragID);
        glLinkProgram(this->ID);
        
        glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
        
        //shader program linking fails
        if(!success) {
            glGetProgramInfoLog(this->ID, 512, NULL, infolog);
            std::cout << "ERROR | Failed to link shader program : " << infolog << std::endl;
        }
        
        //delete shader objects
        glDeleteShader(vertexID);
        glDeleteShader(fragID);
        
        delete shaderCode;
    }
};

#endif
