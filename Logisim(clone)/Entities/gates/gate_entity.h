#ifndef GATE_ENTITY_H
#define GATE_ENTITY_H

#include "../entity.h"
#include "../../enitity_parser.h"
#include "gate_exception.h"
#include <vector>

enum class SocketState {
    ERROR,
    UNINITIALIZED,
    LOW,
    HIGH
};

enum class Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

struct GateFromScript {
    const char* name;
    Texture texture;
    short int width;
    short int height;
    Direction direction;
    short int numInputs;
    std::vector<std::pair<short int, short int>> inputOffsets;
    std::pair<short int, short int> outputOffset;
};

std::vector<std::pair<short int, short int>> stringToList(std::string str)
{
    std::vector<std::pair<short int, short int>> list;
    
    std::stringstream ss(str);
    std::string offset;
    while(std::getline(ss, offset, ')')) {
        std::stringstream ssoffset(offset);
        short int x, y;
        
        while(ssoffset.peek() == ' ' || ssoffset.peek() == '(' || ssoffset.peek() == ',') {
            ssoffset.ignore();
            }
        ssoffset >> x;
            
        while(ssoffset.peek() == ' ' || ssoffset.peek() == ',') {
            ssoffset.ignore();
        }
        ssoffset >> y;
        
        list.push_back(std::make_pair(x, y));
    }
    
    return list;
}

Direction stringToDirection(std::string str)
{
    if(str == "NORTH") {
        return Direction::NORTH;
    }
    if(str == "SOUTH") {
        return Direction::SOUTH;
    }
    if(str == "WEST") {
        return Direction::WEST;
    }
    
    return Direction::EAST;
}

GateFromScript* parseScriptToGate(const char* scriptName)
{
    std::map<std::string, std::string> attributes;
    if(ParseEntity(scriptName, attributes)) {
        return new GateFromScript{
            attributes["name"].c_str(),
            Texture(attributes["image_path"].c_str(), TextureFormats::PNG, true),
            static_cast<short int>(std::stoi(attributes["width"])),
            static_cast<short int>(std::stoi(attributes["height"])),
            stringToDirection(attributes["direction"]),
            static_cast<short int>(std::stoi(attributes["number_input_gates"])),
            stringToList(attributes["input_gates_offsets"]),
            stringToList(attributes["output_gate_offset"])[0]
        };
    }
    else {
        throw GateException("Failed to parse gate script");
    }
}

class Socket {
public:
    Socket(glm::ivec2 position, SocketState state) : m_position{position}, m_state{state} { }
    Socket(glm::ivec2 position) : m_position{position}, m_state{SocketState::UNINITIALIZED} { }
    Socket() : m_position{glm::ivec2(0, 0)}, m_state{SocketState::UNINITIALIZED} { }
    ~Socket() { }
    
private:
    glm::ivec2 m_position;
    SocketState m_state;
    
};

class GateEntity : public Entity {
public:
    inline static unsigned int m_VBO = 0, m_VAO = 0, m_EBO = 0;
    
    GateEntity(Shader& shader, Texture& texture, glm::vec3 position, glm::ivec2 gridPosition)
    :m_direction{Direction::EAST}, m_gridPosition{gridPosition}, Entity(shader, texture, position) {
        if(m_VBO == 0 && m_VAO == 0 && m_EBO == 0)
            this->setup();
    }
    ~GateEntity() { }
    
    void Draw(const glm::mat4& view, const glm::mat4& projection) const override {
        m_shader.Use();
        glBindVertexArray(m_VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);
        model = glm::scale(model, glm::vec3(m_texture.GetTexWidth(), m_texture.GetTexHeight(), 1));
        m_shader.SetMatrix4f("model", model);
        m_shader.SetMatrix4f("view", view);
        m_shader.SetMatrix4f("projection", projection);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    
    virtual SocketState LogicFunction() = 0;
    virtual const char* GetName() const = 0;
    
protected:
    glm::ivec2 m_gridPosition;
    Direction m_direction;
    std::vector<Socket> m_inputs;
    Socket m_output;
    
    void InitializeTexture(Texture& texture) {
        m_texture = texture;
    }
    
    void InitializeInputs(GateFromScript& gate) {
        for(auto& [x, y] : gate.inputOffsets) {
            
            switch(m_direction) {
                case Direction::NORTH:
                    m_inputs.push_back(Socket(glm::ivec2(m_gridPosition.x + y, m_gridPosition.y - x)));
                    break;
                case Direction::SOUTH:
                    m_inputs.push_back(Socket(glm::ivec2(m_gridPosition.x - y, m_gridPosition.y + x)));
                    break;
                case Direction::EAST:
                    m_inputs.push_back(Socket(glm::ivec2(m_gridPosition.x + x, m_gridPosition.y + y)));
                    break;
                case Direction::WEST:
                    m_inputs.push_back(Socket(glm::ivec2(m_gridPosition.x - x, m_gridPosition.y - y)));
                    break;
            }
        }
    }
    
    void InitializeOutput(GateFromScript& gate) {
        int x = gate.outputOffset.first;
        int y = gate.outputOffset.second;
        
        switch(m_direction) {
            case Direction::NORTH:
                m_output = Socket(glm::ivec2(m_gridPosition.x + y, m_gridPosition.y - x));
                break;
            case Direction::SOUTH:
                m_output = Socket(glm::ivec2(m_gridPosition.x - y, m_gridPosition.y + x));
                break;
            case Direction::EAST:
                m_output = Socket(glm::ivec2(m_gridPosition.x + x, m_gridPosition.y + y));
                break;
            case Direction::WEST:
                m_output = Socket(glm::ivec2(m_gridPosition.x - x, m_gridPosition.y - y));
                break;
        }
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
