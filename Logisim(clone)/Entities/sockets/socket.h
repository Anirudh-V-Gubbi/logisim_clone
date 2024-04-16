#ifndef SOCKET_H
#define SOCKET_H

#include <Shaders/shader_manager.h>
#include <Logger/log.h>

#include <functional>
#include <memory>

enum class SocketState {
    ERROR,
    UNINITIALIZED,
    LOW,
    HIGH
};

inline std::ostream& operator<<(std::ostream& os, const SocketState& state) {
    using ss = SocketState;
    switch(state){
        case ss::ERROR:
            os << "ERROR";
            break;
        case ss::UNINITIALIZED:
            os << "UNINITIALIZED";
            break;
        case ss::LOW:
            os << "LOW";
            break;
        case ss::HIGH:
            os << "HIGH";
            break;
    }

    return os;
}

static const std::map<SocketState, glm::vec3> socketStateColorMap = {
    {SocketState::ERROR,         glm::vec3(188.0f / 255, 10.0f / 255, 0.0f / 255)},
    {SocketState::UNINITIALIZED, glm::vec3(37.0f / 255, 17.0f / 255, 255.0f / 255)},
    {SocketState::LOW,           glm::vec3(19.0f / 255, 100.0f / 255, 0.0f / 255)},
    {SocketState::HIGH,          glm::vec3(51.0f / 255, 211.0f / 255, 0.0f / 255)},
};

class Socket {
public:
    Socket(glm::ivec2 position, glm::vec2 absPosition, SocketState state) : m_position{position}, m_absPosition{absPosition}, m_state{state}, m_entityCallback{nullptr} {
        LOG_FUNCTION(this, position, absPosition, state);

        m_color = socketStateColorMap.at(m_state);
    }
    Socket(glm::ivec2 position, glm::vec2 absPosition) : m_position{position}, m_absPosition{absPosition},  m_state{SocketState::UNINITIALIZED}, m_entityCallback{nullptr} {
        LOG_FUNCTION(this, position, absPosition);

        m_color = socketStateColorMap.at(m_state);
    }
    Socket() : m_position{glm::ivec2(0, 0)}, m_state{SocketState::UNINITIALIZED}, m_entityCallback{nullptr} {
        LOG_FUNCTION(this);

        m_color = socketStateColorMap.at(m_state);
    }
    ~Socket() {
        LOG_FUNCTION(this);
    }
    
    glm::ivec2 GetPosition() const {
        LOG_FUNCTION(this);

        return this->m_position;
    }
    
    glm::vec2 GetAbsPosition() const {
        LOG_FUNCTION(this);

        return this->m_absPosition;
    }
    
    glm::vec3 GetColor() const {
        LOG_FUNCTION(this);

        return this->m_color;
    }
    
    SocketState GetState() const {
        LOG_FUNCTION(this);

        return this->m_state;
    }
    
    void ChangeState(SocketState newState, bool notifyCallback = true) {
        LOG_FUNCTION(this, newState, notifyCallback);

        if(m_state != newState) {
            m_state = newState;
            m_color = socketStateColorMap.at(m_state);
            if(m_entityCallback && notifyCallback) (*m_entityCallback)(newState);
        }
    }
    
    void RegisterChangeCallback(std::shared_ptr<std::function<void(SocketState)>> entityCallback) {
        LOG_FUNCTION(this);

        m_entityCallback = entityCallback;
    }

    friend inline std::ostream& operator<<(std::ostream& os, const Socket& socket);
    
private:
    glm::ivec2 m_position;
    glm::vec2 m_absPosition;
    SocketState m_state;
    glm::vec3 m_color;
    std::shared_ptr<std::function<void(SocketState)>> m_entityCallback;
};

inline std::ostream& operator<<(std::ostream& os, const Socket& socket) {
    os << "Socket " << socket.m_position;

    return os;
}

#endif
