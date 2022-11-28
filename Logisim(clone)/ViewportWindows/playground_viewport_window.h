#ifndef PLAYGROUND_VIEWPORT_WINDOW_H
#define PLAYGROUND_VIEWPORT_WINDOW_H

#include "viewport_window.h"
#include "../Shaders/shader.h"

class PlaygroundViewportWindow : public ViewportWindow {
public:
    PlaygroundViewportWindow(unsigned int width, unsigned int height, Shader& m_frameBufferShader)
        : ViewportWindow(width, height, m_frameBufferShader) {
        
    }
    ~PlaygroundViewportWindow() override {
        
    }
    
};

#endif
