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

#include <iostream>
#include <memory>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const glm::ivec2 SCREEN_DIMENSIONS(1000, 675);

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
    Shader frameBufferShader("Shaders/framebuffer_vertex.vs", "Shaders/framebuffer_fragment.fs");
    
    // create a sample entity to render, deleted later by the renderer
    // ---------------------------------------------------------------
    RectangleEntity *r = new RectangleEntity(shader, texture);
    
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
    
    // add entities to the playground
    // ------------------------------
    pWindow->AddEntititesToViewport(*r);
    
    
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        pWindow->Render();
        tWindow->Render();
        psWindow->Render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
