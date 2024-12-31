#include <glad/glad.h>
#include "window.h"

struct CENGINE_Window {
    GLFWwindow *native_window;
};

void CENGINE_WINDOW_Init(size_t pixel_width, size_t pixel_height, const char *name, CENGINE_Window window) 
{
    glfwInit(); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CENGINE_GPU__PROFILE, GLFW_CENGINE_GPU__CORE_PROFILE);
    glfwWindowHint(GLFW_CENGINE_GPU__FORWARD_COMPAT, GL_TRUE);  // Required on macOS
    window->native_window = glfwCreateWindow(pixel_width, pixel_height, name, NULL, NULL); 
    glfwMakeContextCurrent(window->native_window); 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
        fprintf(stderr, "Failed to initialize GLAD\n"); 
    } 
}

bool CENGINE_WINDOW_IsRunning(CENGINE_Window window) 
{
    return !glfwWindowShouldClose(window->native_window);
}

void CENGINE_WINDOW_ListenToUserInput(CENGINE_Window window) 
{
    glfwPollEvents();
}

void CENGINE_WINDOW_FinishFrame(CENGINE_Window window) 
{
    glfwSwapBuffers(window->native_window);
}


void CENGINE_WINDOW_Destroy(CENGINE_Window window) 
{
    glfwDestroyWindow(window->native_window);
    glfwTerminate();
}