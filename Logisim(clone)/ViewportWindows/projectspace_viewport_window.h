#ifndef PROJECTSPACE_VIEWPORT_WINDOW_H
#define  PROJECTSPACE_VIEWPORT_WINDOW_H

#include "viewport_window.h"
#include "../Shaders/shader.h"

class ProjectspaceViewportWindow : public ViewportWindow {
public:
    ProjectspaceViewportWindow(glm::vec2 fractionalWindowDimensions, glm::vec2 fractionalPosition,
        const glm::ivec2& screenDimensions, Shader& shader)
        : ViewportWindow(fractionalWindowDimensions, fractionalPosition, screenDimensions, shader) {
        
    }
    ~ProjectspaceViewportWindow() override {
        
    }
    
};

#endif
