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
            Texture(attributes["image_path"].c_str()),
            static_cast<short int>(std::stoi(attributes["width"])),
            static_cast<short int>(std::stoi(attributes["height"])),
            stringToDirection(attributes["direction"]),
            static_cast<short int>(std::stoi(attributes["number_input_gates"])),
            stringToList(attributes["input_gates_offsets"]),
            stringToList(attributes["output_gate_offset"])[0]
        };
    }
    else {
        throw GateException("Failed to parse And Gate script");
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
    GateEntity(Shader& shader, Texture& texture, glm::vec3 position, glm::ivec2 gridPosition)
    :m_direction{Direction::EAST}, m_gridPosition{gridPosition}, Entity(shader, texture, position) {
        
    }
    ~GateEntity() { }
    
    void Draw(const glm::mat4& view, const glm::mat4& projection) const override {
        
    }
    
    virtual SocketState LogicFunction() = 0;
    virtual const char* GetName() const { return this->m_name; }
    
protected:
    const char* m_name;
    glm::ivec2 m_gridPosition;
    Direction m_direction;
    std::vector<Socket> m_inputs;
    Socket m_output;
};
#endif
