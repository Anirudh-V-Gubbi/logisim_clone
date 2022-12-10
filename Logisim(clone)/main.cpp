#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include "Shaders/shader.h"
#include "Textures/texture.h"
#include "Entities/rectangle_entity.h"
#include "renderer.h"
#include "ViewportWindows/viewport_window.h"
#include "ViewportWindows/playground_viewport_window.h"
#include "framebuffer.h"
#include "ViewportWindows/toolbar_viewport_window.h"
#include "ViewportWindows/projectspace_viewport_window.h"
#include "Logger/logger.h"
#include "Entities/global_grid.h"
#include "Event/key_event.h"
#include "Event/application_event.h"
#include "Event/mouse_event.h"
#include "Event/event_handler.h"

#include <iostream>
#include <memory>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// screen dimensions and view and projection matrices
const glm::ivec2 SCREEN_DIMENSIONS(1000, 675);
const glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0f));
const glm::mat4 projectionMatrix = glm::ortho<float>(0.0f, SCREEN_DIMENSIONS.x, SCREEN_DIMENSIONS.y, 0.0f, -0.1f, 100.0f);

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

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREEN_DIMENSIONS.x, SCREEN_DIMENSIONS.y, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
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
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // set up shaders
    // --------------
    Shader shader("Shaders/vertex.vs", "Shaders/frag.fs");
    Texture texture("Textures/demo_image.jpg");
    Texture dummyTexture("Textures/dummy.jpg");
    Shader frameBufferShader("Shaders/framebuffer_vertex.vs", "Shaders/framebuffer_fragment.fs");
    Shader gridShader("Shaders/grid_vertex.vs", "Shaders/grid_fragment.fs");
    
    // create a sample entity to render, deleted later by the renderer
    // ---------------------------------------------------------------
    //RectangleEntity *r = new RectangleEntity(shader, texture, glm::vec3(100.0, 100.0, 0.0));
    
    // an enum to access viewport windows
    // ----------------------------------
    enum ViewportWindows{
        TOOLBAR = 0,
        PROJECT_SPACE = 1,
        PLAYGROUND = 2
    };
    
    // static array that manages the viewport windows
    // ----------------------------------------------
    std::unique_ptr<ViewportWindow> viewports[3];
    
    // create a toolbar viewport window, where tools and menu options will be rendered
    // -------------------------------------------------------------------------------
    ToolbarViewportWindow* tWindow = new ToolbarViewportWindow(glm::vec2(1.0,0.1), glm::vec2(0.0f, 0.0f), SCREEN_DIMENSIONS, frameBufferShader);
    
    // create a project space viewport window, where circuit projects will be rendered
    // -------------------------------------------------------------------------------
    ProjectspaceViewportWindow* psWindow = new ProjectspaceViewportWindow(glm::vec2(0.2, 0.9), glm::vec2(0.0f, 0.1f), SCREEN_DIMENSIONS, frameBufferShader);
    
    // create a playground viewport window, where logic gates are expected to
    // be rendered
    // ----------------------------------------------------------------------
    PlaygroundViewportWindow* pWindow = new PlaygroundViewportWindow(glm::vec2(0.8, 0.9), glm::vec2(0.2f, 0.1f), SCREEN_DIMENSIONS, frameBufferShader);
    
    // push the window pointers onto the static array
    // ----------------------------------------------
    viewports[ViewportWindows::TOOLBAR].reset(tWindow);
    viewports[ViewportWindows::PROJECT_SPACE].reset(psWindow);
    viewports[ViewportWindows::PLAYGROUND].reset(pWindow);
    
    // create a grid system
    // --------------------
    GlobalGrid *grid = new GlobalGrid(gridShader, dummyTexture, glm::vec3(0.0, 0.0, 0.0), glm::vec2(pWindow->GetWindowDimensions().x * SCREEN_DIMENSIONS.x, pWindow->GetWindowDimensions().y * SCREEN_DIMENSIONS.y));
    
    // add entities to the playground
    // ------------------------------
    //viewports[ViewportWindows::PLAYGROUND]->AddEntititesToViewport(*r);
    viewports[ViewportWindows::PLAYGROUND]->AddEntititesToViewport(*grid);
    
    Logger* logger = new Logger("Logger");
    
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----

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
        KeyPressedEvent event(key, 1);
        EventHandler::DispatchEvent(event);
    }
    else if(action == GLFW_RELEASE) {
        KeyReleasedEvent event(key);
        EventHandler::DispatchEvent(event);
    }
}

// process mouse movements
// -----------------------
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    MouseMovedEvent event(xpos, ypos);
    EventHandler::DispatchEvent(event);
}

// process mouse button clicks
// ---------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(action == GLFW_PRESS) {
        MouseButtonPressedEvent event(button);
        EventHandler::DispatchEvent(event);
    }
    else if(action == GLFW_RELEASE) {
        MouseButtonReleasedEvent event(button);
        EventHandler::DispatchEvent(event);
    }
}

// process mouse scrolls
// ---------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    MouseScrolledEvent event(xoffset, yoffset);
    EventHandler::DispatchEvent(event);
}
