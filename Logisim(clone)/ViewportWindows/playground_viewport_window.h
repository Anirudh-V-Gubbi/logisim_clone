#ifndef PLAYGROUND_VIEWPORT_WINDOW_H
#define PLAYGROUND_VIEWPORT_WINDOW_H

#include "viewport_window.h"
#include "../Shaders/shader.h"

class PlaygroundViewportWindow : public ViewportWindow {
public:
    PlaygroundViewportWindow(glm::ivec2 windowDimensions, glm::vec2 fractionalPosition,
        glm::ivec2 screenDimensions, Shader& shader)
        : ViewportWindow(windowDimensions, fractionalPosition, screenDimensions, shader) {
        
    }
    ~PlaygroundViewportWindow() override {
        
    }
    
};

#endif
