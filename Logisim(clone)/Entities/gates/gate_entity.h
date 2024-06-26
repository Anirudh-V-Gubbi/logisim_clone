#ifndef GATE_ENTITY_H
#define GATE_ENTITY_H

#include <Entities/entity.h>
#include <enitity_parser.h>
#include <Entities/entity_exception.h>
#include <Entities/sockets/gate_sockets.h>
#include <Entities/global_grid.h>
#include <Logger/log.h>
#include <utility.h>
#include <vector>
#include <map>

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
        throw EntityException("Failed to parse gate script");
    }
}

class GateEntity : public Entity {
public:
    inline static unsigned int m_VBO = 0, m_VAO = 0, m_EBO = 0;
    
    virtual ~GateEntity() {
        LOG_FUNCTION(this);
    }
    
    void Draw(const glm::mat4& view, const glm::mat4& projection) const override {
        LOG_FUNCTION(this);

        m_shader->Use();
        glBindVertexArray(m_VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);
        model = glm::scale(model, glm::vec3(m_texture.GetTexWidth(), m_texture.GetTexHeight(), 1));
        m_shader->SetMatrix4f("model", model);
        m_shader->SetMatrix4f("view", view);
        m_shader->SetMatrix4f("projection", projection);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Draw I/O socket indicators
        // --------------------------
        GLint currentTex = currentTextureID;
        m_sockets.Draw(view, projection);
        glBindTexture(GL_TEXTURE_2D, currentTex);
    }
    
    void OnInputChange(SocketState newState) {
        LOG_FUNCTION(this, newState);

        SocketState finalState = LogicFunction();
        
        if(m_sockets.m_outputs[0]->GetState() != finalState) {
            m_sockets.m_outputs[0]->ChangeState(finalState);
            GlobalGrid::GetGrid()->PushStateChangeNotification(*m_sockets.m_outputs[0].get());
        }
    }
    
    virtual SocketState LogicFunction() const = 0;
    virtual const char* GetName() const = 0;
    
protected:
    glm::ivec2 m_gridPosition;
    Direction m_direction;
    GateSockets m_sockets;
    std::shared_ptr<std::function<void(SocketState)>> m_onInputChange;
    
    // Protected constructor for the abstract class
    // --------------------------------------------
    GateEntity(std::shared_ptr<Shader> shader, Texture& texture, glm::vec3 position, glm::ivec2 gridPosition)
    :m_direction{Direction::EAST}, m_gridPosition{gridPosition}, Entity(shader, texture, position) {
        LOG_FUNCTION(this, shader, texture, position, gridPosition);

        if(m_VBO == 0 && m_VAO == 0 && m_EBO == 0)
            this->setup();
        m_onInputChange = std::make_shared<std::function<void(SocketState)>>([this](SocketState newState) {
            this->OnInputChange(newState);
        });
    }

    void InitializeTexture(Texture& texture) {
        LOG_FUNCTION(this, texture);

        m_texture = texture;
    }
    
    void InitializeInputs(GateFromScript& gate) {
        LOG_FUNCTION(this, gate.name);
        
        for(auto& [x, y] : gate.inputOffsets) {
            glm::ivec2 position;
            glm::vec2 absPosition;
            
            switch(m_direction) {
                case Direction::NORTH:
                    position = glm::ivec2(m_gridPosition.x - x, m_gridPosition.y + y);
                    absPosition = GlobalGrid::GetGrid()->GetGridPointPositionRelative(m_position, x, y);
                    break;
                case Direction::SOUTH:
                    position = glm::ivec2(m_gridPosition.x + x, m_gridPosition.y - y);
                    absPosition = GlobalGrid::GetGrid()->GetGridPointPositionRelative(m_position, x, y);
                    break;
                case Direction::EAST:
                    position = glm::ivec2(m_gridPosition.x + y, m_gridPosition.y + x);
                    absPosition = GlobalGrid::GetGrid()->GetGridPointPositionRelative(m_position, y, x);
                    break;
                case Direction::WEST:
                    position = glm::ivec2(m_gridPosition.x - y, m_gridPosition.y - x);
                    absPosition = GlobalGrid::GetGrid()->GetGridPointPositionRelative(m_position, y, x);
                    break;
            }
            
            m_sockets.m_inputs.push_back(std::make_shared<Socket>(position, absPosition,
                                            GlobalGrid::GetGrid()->GetSocketStateAt(position)));
            m_sockets.m_inputs.back()->RegisterChangeCallback(m_onInputChange);
        }
        
        GlobalGrid::GetGrid()->AddSocketsToBoard(m_sockets.m_inputs);
    }
    
    void InitializeOutput(GateFromScript& gate) {
        LOG_FUNCTION(this, gate.name);

        int x = gate.outputOffset.first;
        int y = gate.outputOffset.second;
        glm::ivec2 position;
        glm::vec2 absPosition;
        
        switch(m_direction) {
            case Direction::NORTH:
                position = glm::ivec2(m_gridPosition.x - x, m_gridPosition.y + y);
                absPosition = GlobalGrid::GetGrid()->GetGridPointPositionRelative(m_position, x, y);
                break;
            case Direction::SOUTH:
                position = glm::ivec2(m_gridPosition.x + x, m_gridPosition.y - y);
                absPosition = GlobalGrid::GetGrid()->GetGridPointPositionRelative(m_position, x, y);
                break;
            case Direction::EAST:
                position = glm::ivec2(m_gridPosition.x + y, m_gridPosition.y + x);
                absPosition = GlobalGrid::GetGrid()->GetGridPointPositionRelative(m_position, y, x);
                break;
            case Direction::WEST:
                position = glm::ivec2(m_gridPosition.x - y, m_gridPosition.y - x);
                absPosition = GlobalGrid::GetGrid()->GetGridPointPositionRelative(m_position, y, x);
                break;
        }
        
        m_sockets.m_outputs.push_back(std::make_shared<Socket>(position, absPosition, LogicFunction()));
        
        GlobalGrid::GetGrid()->AddSocketsToBoard(m_sockets.m_outputs);
    }
    
private:
    void setup() {
        LOG_FUNCTION(this);

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
