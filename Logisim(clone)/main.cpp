#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include "Shaders/shader.h"
#include "Textures/texture.h"
#include "ViewportWindows/viewport_window.h"
#include "ViewportWindows/playground_viewport_window.h"
#include "ViewportWindows/toolbar_viewport_window.h"
#include "ViewportWindows/projectspace_viewport_window.h"
#include "Logger/logger.h"
#include "Entities/global_grid.h"
#include "Event/key_event.h"
#include "Event/application_event.h"
#include "Event/mouse_event.h"
#include "Event/event_handler.h"
#include "Shaders/shader_manager.h"

#include <iostream>
#include <memory>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// screen dimensions and view and projection matrices
const glm::ivec2 SCREEN_DIMENSIONS(1000, 675);
const glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0f));
const glm::mat4 projectionMatrix = glm::ortho<float>(0.0f, SCREEN_DIMENSIONS.x, SCREEN_DIMENSIONS.y, 0.0f, -0.1f, 100.0f);

// static array that manages the viewport windows
// ----------------------------------------------
std::vector<std::unique_ptr<ViewportWindow>> viewports(3);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Initialise the logger
    new Logger("Logger");

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREEN_DIMENSIONS.x, SCREEN_DIMENSIONS.y, "Logisim(clone)", NULL, NULL);
    if (window == NULL)
    {
        Logger::GetInstance()->error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (glewInit() != GLEW_OK)
    {
        Logger::GetInstance()->error("Failed to initialize GLEW");
        return -1;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // an enum to access viewport windows
    // ----------------------------------
    enum ViewportWindows{
        TOOLBAR = 0,
        PROJECT_SPACE = 1,
        PLAYGROUND = 2
    };
    
    // create a toolbar viewport window, where tools and menu options will be rendered
    // -------------------------------------------------------------------------------
    ToolbarViewportWindow* tWindow = new ToolbarViewportWindow(glm::vec2(1.0,0.1), glm::vec2(0.0f, 0.0f),
                                            SCREEN_DIMENSIONS, ShaderManager::GetShader("framebuffer"));
    
    // create a project space viewport window, where circuit projects will be rendered
    // -------------------------------------------------------------------------------
    ProjectspaceViewportWindow* psWindow = new ProjectspaceViewportWindow(glm::vec2(0.2, 0.9), glm::vec2(0.0f, 0.1f),
                                                    SCREEN_DIMENSIONS, ShaderManager::GetShader("framebuffer"));
    
    // create a playground viewport window, where logic gates are expected to
    // be rendered
    // ----------------------------------------------------------------------
    PlaygroundViewportWindow* pWindow = new PlaygroundViewportWindow(glm::vec2(0.8, 0.9), glm::vec2(0.2f, 0.1f),
                                                SCREEN_DIMENSIONS, ShaderManager::GetShader("framebuffer"));
    
    // push the window pointers onto the static array
    // ----------------------------------------------
    viewports[ViewportWindows::TOOLBAR].reset(tWindow);
    viewports[ViewportWindows::PROJECT_SPACE].reset(psWindow);
    viewports[ViewportWindows::PLAYGROUND].reset(pWindow);
    
    // create a grid system
    // --------------------
    std::shared_ptr<GlobalGrid> grid = std::make_shared<GlobalGrid>(ShaderManager::GetShader("grid"), *EmptyTexture::GetInstance(),
                                glm::vec3(0.2f * SCREEN_DIMENSIONS.x, 0.1f * SCREEN_DIMENSIONS.y, 0.0),
                                glm::vec2(pWindow->GetWindowDimensions().x * SCREEN_DIMENSIONS.x, pWindow->GetWindowDimensions().y * SCREEN_DIMENSIONS.y));
    
    // add entities to the playground
    // ------------------------------
    pWindow->SetGrid(grid);
    viewports[ViewportWindows::PLAYGROUND]->AddEntititesToViewport(grid);
    
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        GlobalGrid::GetGrid()->Update();
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        for(auto& vWindow : viewports) {
            vWindow->Render(viewMatrix, projectionMatrix);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// process key inputs
// ------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS || action == GLFW_REPEAT) {
        if(key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, 1);
        }
        else {
            KeyPressedEvent event(key, 1);
            EventHandler::DispatchEvent(event, viewports);
        }
    }
    else if(action == GLFW_RELEASE) {
        KeyReleasedEvent event(key);
        EventHandler::DispatchEvent(event, viewports);
    }
}

// process mouse movements
// -----------------------
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    MouseMovedEvent event(xpos, ypos);
    EventHandler::DispatchEvent(event, viewports);
}

// process mouse button clicks
// ---------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    
    if(action == GLFW_PRESS) {
        MouseButtonPressedEvent event(button, xPos, yPos);
        EventHandler::DispatchEvent(event, viewports);
    }
    else if(action == GLFW_RELEASE) {
        MouseButtonReleasedEvent event(button, xPos, yPos);
        EventHandler::DispatchEvent(event, viewports);
    }
}

// process mouse scrolls
// ---------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    MouseScrolledEvent event(xoffset, yoffset);
    EventHandler::DispatchEvent(event, viewports);
}
