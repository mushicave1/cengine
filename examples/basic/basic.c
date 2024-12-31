#include <stdio.h>
#include <cengine/cengine.h>

float vertices[32] = {
    -0.5, -0.5, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 
    0.5, -0.5, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
    0.5, 0.5, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0,
    -0.5, 0.5, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0,
}; 


uint32_t indices[6] = {
    0, 1, 2,
    2, 3, 0
};

void keyCallback(GLFWwindow* window, int key, int action, int mods, int scancode) {
    if(key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, char **argv) {
    CENGINE_Window window = NULL;
    CENGINE_WINDOW_Init(600, 400, "cengine", &window);
    
    CENGINE_GPU_VertexBuffer vbo = CENGINE_GPU_CreateVertexBuffer((void*)vertices, sizeof(vertices));
    CENGINE_GPU_IndexBuffer ebo = CENGINE_GPU_CreateIndexBuffer((void*)indices, sizeof(indices));
    CENGINE_GPU_Program program = CENGINE_GPU_CreateProgram(CENGINE_CORE_ReadFile("examples/shaders/vs_basic.glsl"), CENGINE_CORE_ReadFile("examples/shaders/fs_color.glsl"));

    size_t oak_planks_image_width;
    size_t oak_planks_image_height;
    size_t oak_planks_image_channels;
    char *oak_planks_image_pixels = NULL;
    CENGINE_IMAGE_LOADER_Load("examples/textures/oak_planks.png", &oak_planks_image_width, &oak_planks_image_height, &oak_planks_image_channels, &oak_planks_image_pixels);
    CENGINE_GPU_TextureFormat oak_planks_texture_format = CENGINE_GPU_TEXTURE_FORMAT_RGB;
    CENGINE_GPU_TextureInternalFormat oak_planks_texture_internal_format = CENGINE_GPU_TEXTURE_INTERNAL_FORMAT_RGB32;
    CENGINE_GPU_Texture oak_planks_texture = CENGINE_GPU_CreateTexture(
        oak_planks_image_pixels, 
        oak_planks_image_width, 
        oak_planks_image_height,
        oak_planks_texture_format,
        oak_planks_texture_internal_format,
        0 
    );

    const uint32_t offsets[3] = {0, 3*sizeof(float), 6*sizeof(float)};
    const uint32_t element_counts[3] = {3, 3, 2}; 
    CENGINE_GPU_BufferView view;
    view.offsets = offsets;
    view.len = 3;
    view.stride = 8 * sizeof(float);
    view.element_counts = element_counts;

    CENGINE_GPU_VertexInput vao = CENGINE_GPU_CreateVertexInput(vbo, view, ebo);
    
    while (CENGINE_WINDOW_IsRunning(window)) {
        CENGINE_WINDOW_ListenToUserInput(window);

        CENGINE_GPU_DrawIndexed(6, vao, program, oak_planks_texture);

        CENGINE_WINDOW_FinishFrame(window);
    }

    CENGINE_GPU_DestroyVertexBuffer(&vbo);
    CENGINE_GPU_DestroyIndexBuffer(&ebo);
    CENGINE_GPU_DestroyProgram(&program);
    CENGINE_GPU_DestroyVertexInput(&vao);
    CENGINE_GPU_DestroyTexture(&oak_planks_texture);
    CENGINE_WINDOW_Destroy(window);
    
    return 0;
} 