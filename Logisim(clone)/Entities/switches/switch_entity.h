#ifndef INPUT_SWITCH_H
#define INPUT_SWITCH_H

#include "../entity.h"
#include "../../enitity_parser.h"
#include "../sockets/switch_sockets.h"

struct SwitchFromScript {
    const char* name;
    Texture texture;
    Texture colorMapTexture;
    short int width;
    short int height;
    Direction direction;
    std::vector<std::pair<short int, short int>> switchSocketOffsets;
};

SwitchFromScript* parseScriptToSwitch(const char* scriptName)
{
    std::map<std::string, std::string> attributes;
    if(ParseEntity(scriptName, attributes)) {
        return new SwitchFromScript{
            attributes["name"].c_str(),
            Texture(attributes["image_path"].c_str(), TextureFormats::PNG, true),
            Texture(attributes["color_map_path"].c_str(), TextureFormats::PNG, true),
            static_cast<short int>(std::stoi(attributes["width"])),
            static_cast<short int>(std::stoi(attributes["height"])),
            stringToDirection(attributes["direction"]),
            stringToList(attributes["switch_socket_offsets"]),
        };
    }
    else {
        throw EntityException("Failed to parse switch script: ");
    }
}

class SwitchEntity : public Entity {
public:
    inline static unsigned int m_VBO = 0, m_VAO = 0, m_EBO = 0;
    
    SwitchEntity(Shader& shader, Texture& texture, glm::vec3 position, glm::ivec2 gridPosition)
    :m_direction{Direction::EAST}, m_gridPosition{gridPosition}, Entity(shader, texture, position) {
        if(m_VBO == 0 && m_VAO == 0 && m_EBO == 0)
            this->setup();
    }
    ~SwitchEntity() { }
    
    void Draw(const glm::mat4& view, const glm::mat4& projection) const override {
        m_shader.Use();
        glBindVertexArray(m_VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);
        model = glm::scale(model, glm::vec3(m_texture.GetTexWidth(), m_texture.GetTexHeight(), 1));
        m_shader.SetMatrix4f("model", model);
        m_shader.SetMatrix4f("view", view);
        m_shader.SetMatrix4f("projection", projection);
        m_shader.SetVector3f("switchColor", socketStateColorMap.at(m_switchState));
        m_shader.SetInteger1i("switchTexture", 0);
        m_shader.SetInteger1i("colorMapTexture", 1);
        
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, m_colorMapTexture.ID);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
        
        // Draw I/O socket indicators
        // --------------------------
        GLint currentTex = currentTextureID;
        m_sockets.Draw(view, projection);
        glBindTexture(GL_TEXTURE_2D, currentTex);
    }
    
    virtual const char* GetName() const = 0;
    virtual void OnInputChange(SocketState newState) {};
    
protected:
    glm::ivec2 m_gridPosition;
    Direction m_direction;
    SwitchSockets m_sockets;
    Texture m_colorMapTexture;
    SocketState m_switchState;
    std::function<void(SocketState)> m_onInputChange;
    
    void InitializeSwitchEntity(SwitchFromScript& sswitch) {
        m_onInputChange = [this](SocketState newState) {this->OnInputChange(newState);};
        
        m_texture = sswitch.texture;
        m_colorMapTexture = sswitch.colorMapTexture;

        for(auto& [x, y] : sswitch.switchSocketOffsets) {
            
            switch(m_direction) {
                case Direction::NORTH:
                    m_sockets.m_sockets.push_back(Socket(glm::ivec2(m_gridPosition.x - x, m_gridPosition.y + y), GlobalGrid::GetGrid()->GetGridPointPositionRelative(m_position, x, y), m_switchState, &m_onInputChange));
                    break;
                case Direction::SOUTH:
                    m_sockets.m_sockets.push_back(Socket(glm::ivec2(m_gridPosition.x + x, m_gridPosition.y - y), GlobalGrid::GetGrid()->GetGridPointPositionRelative(m_position, x, y), m_switchState, &m_onInputChange));
                    break;
                case Direction::EAST:
                    m_sockets.m_sockets.push_back(Socket(glm::ivec2(m_gridPosition.x + y, m_gridPosition.y + x), GlobalGrid::GetGrid()->GetGridPointPositionRelative(m_position, y, x), m_switchState, &m_onInputChange));
                    break;
                case Direction::WEST:
                    m_sockets.m_sockets.push_back(Socket(glm::ivec2(m_gridPosition.x - y, m_gridPosition.y - x), GlobalGrid::GetGrid()->GetGridPointPositionRelative(m_position, y, x), m_switchState, &m_onInputChange));
                    break;
            }
        }
        
        GlobalGrid::GetGrid()->AddSocketsToBoard(m_sockets.m_sockets);
    }
    
private:
    void setup() {
        float vertices[] = {
             // positions // texture coords
             1.0f, 1.0f,  1.0f, 1.0f,   // top right
             1.0f, 0.0f,  1.0f, 0.0f,   // bottom right
             0.0f, 0.0f,  0.0f, 0.0f,   // bottom left
             0.0f, 1.0f,  0.0f, 1.0f    // top left
        };
        unsigned int indices[] = {
            0, 3, 2,  // first Triangle
            0, 2, 1   // second Triangle
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
