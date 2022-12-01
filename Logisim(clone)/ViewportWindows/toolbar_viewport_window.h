#ifndef TOOLBAR_VIEWPORT_WINDOW_H
#define TOOLBAR_VIEWPORT_WINDOW_H

#include "viewport_window.h"
#include "../Shaders/shader.h"

class ToolbarViewportWindow : public ViewportWindow {
public:
    ToolbarViewportWindow(glm::vec2 fractionalWindowDimensions, glm::vec2 fractionalPosition,
        const glm::ivec2& screenDimensions, Shader& shader)
        : ViewportWindow(fractionalWindowDimensions, fractionalPosition, screenDimensions, shader) {
        
    }
    ~ToolbarViewportWindow() override {
        std::cout << "Deleted Toolbar Viewport" << std::endl;
    }
    
};

#endif
