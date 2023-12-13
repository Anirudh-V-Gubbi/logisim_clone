#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <unordered_map>
#include <string>
#include <memory>

#include "shader.h"

class ShaderManager {
public:
    static std::shared_ptr<Shader> GetShader(std::string name) {
        if(m_shaderMap.find(name) == m_shaderMap.end()) {
            m_shaderMap[name] = std::shared_ptr<Shader> (new Shader(name + "_vertex.vs", name + "_fragment.fs"));
        }

        return m_shaderMap[name];
    }

private:
    static inline std::unordered_map<std::string, std::shared_ptr<Shader> > m_shaderMap;
};

#endif