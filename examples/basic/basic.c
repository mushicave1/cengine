#include <stdio.h>
#include <cengine/cengine.h>

float vertices[12] = {
    -0.5, -0.5, 1.0, 
    0.5, -0.5, 1.0,
    0.5, 0.5, 1.0,
    -0.5, 0.5, 1.0
}; 


uint32_t indices[12] = {
    0, 1, 2,
    2, 3, 0
};

void keyCallback(GLFWwindow* window, int key, int action, int mods, int scancode) {
    if(key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, char **argv) {
    CENGINE_Window window;
    CENGINE_WINDOW_Init(600, 400, "cengine", window);
    
    CENGINE_GPU_VertexBuffer vbo = CENGINE_GPU_CreateVertexBuffer((void*)vertices, sizeof(float) * sizeof(vertices));
    CENGINE_GPU_IndexBuffer ebo = CENGINE_GPU_CreateIndexBuffer((void*)indices, sizeof(indices) * sizeof(uint32_t));
    CENGINE_GPU_Program program = CENGINE_GPU_CreateProgram(CENGINE_CORE_ReadFile("examples/shaders/vs_basic.glsl"), CENGINE_CORE_ReadFile("examples/shaders/fs_color.glsl"));

    const uint32_t offsets[3] = {0};
    const uint32_t element_counts[3] = {3}; 
    CENGINE_GPU_BufferView view;
    view.offsets = offsets;
    view.len = 1;
    view.stride = 3 * sizeof(float);
    view.element_counts = element_counts;

    CENGINE_GPU_VertexInput vao = CENGINE_GPU_CreateVertexInput(vbo, view, ebo);
    
    while (CENGINE_WINDOW_IsRunning(window)) {
        CENGINE_WINDOW_ListenToUserInput(window);

        CENGINE_GPU_DrawIndexed(6, vao, program);

        CENGINE_WINDOW_FinishFrame(window);
    }

    CENGINE_GPU_DestroyVertexBuffer(&vbo);
    CENGINE_GPU_DestroyIndexBuffer(&ebo);
    CENGINE_GPU_DestroyProgram(&program);
    CENGINE_GPU_DestroyVertexInput(&vao);
    CENGINE_WINDOW_Destroy(&window);
    
    return 0;
} 