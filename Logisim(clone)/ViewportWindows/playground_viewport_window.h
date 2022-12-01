#ifndef PLAYGROUND_VIEWPORT_WINDOW_H
#define PLAYGROUND_VIEWPORT_WINDOW_H

#include "viewport_window.h"
#include "../Shaders/shader.h"

class PlaygroundViewportWindow : public ViewportWindow {
public:
    PlaygroundViewportWindow(glm::vec2 fractionalWindowDimensions, glm::vec2 fractionalPosition,
        const glm::ivec2& screenDimensions, Shader& shader)
        : ViewportWindow(fractionalWindowDimensions, fractionalPosition, screenDimensions, shader) {
        
    }
    ~PlaygroundViewportWindow() override {
        std::cout << "Deleted Playground Viewport" << std::endl;
    }
    
};

#endif
