#include <cengine/cengine.h>
#include <core/core.h>
#include <glad/glad.h>

#include "gpu.h"

typedef struct { 
    void *vertices;
    GLuint vbo;
} GLVertexBuffer;

typedef struct {
    void *indices;
    GLuint ebo;
} GLIndexBuffer;

typedef struct {
    GLuint program;
} GLProgram;

typedef struct {
    GLuint tex;
    void* pixels;
    GLuint width;
    GLuint height;
    GLuint format;
    GLuint internal_format;
    GLuint mip_map_levels;
} GLTexture;

typedef struct {
    GLuint vao;
} GLVertexInput;

typedef struct {
    GLVertexBuffer vertex_buffers[CENGINE_GPU_MAX_VERTEX_BUFFERS];
    GLIndexBuffer index_buffers[CENGINE_GPU_MAX_INDEX_BUFFERS];
    GLVertexInput vertex_inputs[CENGINE_GPU_MAX_VERTEX_INPUTS];
    GLProgram programs[CENGINE_GPU_MAX_PROGRAMS];
    GLTexture textures[CENGINE_GPU_MAX_TEXTURES];
} CENGINE_GPU_Context;

static CENGINE_GPU_Context s_ctx;


static inline GLenum CENGINE_GPU_RENDERER_TextureFormat(CENGINE_GPU_TextureFormat format)
{
    switch(format) {
        case CENGINE_GPU_TEXTURE_FORMAT_R: return GL_R; break;
        case CENGINE_GPU_TEXTURE_FORMAT_RG: return GL_RG; break;
        case CENGINE_GPU_TEXTURE_FORMAT_RGB: return GL_RGB; break;
        case CENGINE_GPU_TEXTURE_FORMAT_RGBA: return GL_RGBA; break;
    }
}

static inline GLenum CENGINE_GPU_RENDERER_TextureInternalFormat(CENGINE_GPU_TextureInternalFormat internal_format) 
{
    switch(internal_format) {
        case CENGINE_GPU_TEXTURE_INTERNAL_FORMAT_RGB32: return GL_RGB; break;
        case CENGINE_GPU_TEXTURE_INTERNAL_FORMAT_RGB32F: return GL_RGB32F; break;
        case CENGINE_GPU_TEXTURE_INTERNAL_FORMAT_RGBA32: return GL_RGBA; break;
        case CENGINE_GPU_TEXTURE_INTERNAL_FORMAT_RGBA32F: return GL_RGBA32F; break;
    }
}


CENGINE_GPU_VertexBuffer CENGINE_GPU_CreateVertexBuffer(void *data, size_t byte_size) 
{ 
    static uint16_t counter = -1;
    counter++;
    GLVertexBuffer *buffer = &s_ctx.vertex_buffers[counter];
    glGenBuffers(1, &buffer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData(GL_ARRAY_BUFFER, byte_size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return (CENGINE_GPU_VertexBuffer){.idx = counter};
}

void CENGINE_GPU_DestroyVertexBuffer(CENGINE_GPU_VertexBuffer *handle) 
{
    GLVertexBuffer *buffer = &s_ctx.vertex_buffers[handle->idx];
    glDeleteBuffers(1, &buffer->vbo);
    handle->idx = CENGINE_GPU_NULL_INDEX;
}

CENGINE_GPU_IndexBuffer CENGINE_GPU_CreateIndexBuffer(void *data, size_t byte_size) 
{
    static uint16_t counter = -1;
    counter++;
    GLIndexBuffer* buffer = &s_ctx.index_buffers[counter];
    glGenBuffers(1, &buffer->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return (CENGINE_GPU_IndexBuffer){.idx = counter};
}

void CENGINE_GPU_DestroyIndexBuffer(CENGINE_GPU_IndexBuffer *handle) 
{
    GLIndexBuffer *buffer = &s_ctx.index_buffers[handle->idx];
    glDeleteBuffers(1, &buffer->ebo);
    handle->idx = CENGINE_GPU_NULL_INDEX;
}

CENGINE_GPU_VertexInput CENGINE_GPU_CreateVertexInput(CENGINE_GPU_VertexBuffer vertex_buffer, CENGINE_GPU_BufferView buffer_view, CENGINE_GPU_IndexBuffer index_buffer) 
{
    static uint16_t counter = -1;
    counter++;
    GLVertexInput *vao = &s_ctx.vertex_inputs[counter];

    GLVertexBuffer *vbo = &s_ctx.vertex_buffers[vertex_buffer.idx];

    GLIndexBuffer *ebo = &s_ctx.index_buffers[index_buffer.idx];
    glGenVertexArrays(1, &vao->vao);
    glBindVertexArray(vao->vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo->vbo);

    if(CENGINE_GPU_ASSERT_HANDLE_IS_VALID(index_buffer)) {
        GLIndexBuffer *ebo = &s_ctx.index_buffers[index_buffer.idx];
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->ebo);
    }

    for(size_t i = 0; i < buffer_view.len; i++) 
    {
        glVertexAttribPointer(i, buffer_view.element_counts[i], GL_FLOAT, GL_FALSE, buffer_view.stride, (void*)(buffer_view.offsets[i]));
        glEnableVertexAttribArray(i);
    }    

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return (CENGINE_GPU_VertexInput){.idx = counter};
}

void CENGINE_GPU_DestroyVertexInput(CENGINE_GPU_VertexInput *handle) 
{
    GLVertexInput *vertex_input = &s_ctx.vertex_inputs[handle->idx];
    glDeleteVertexArrays(1, &vertex_input->vao);
    handle->idx = CENGINE_GPU_NULL_INDEX; 
}


CENGINE_GPU_Program CENGINE_GPU_CreateProgram(const char *vs_code, const char *fs_code) 
{
    static uint16_t counter = -1;
    counter++;
    GLProgram* program = &s_ctx.programs[counter];
    const GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_shader, 1, &vs_code, NULL);
    glCompileShader(v_shader);
    GLint success;
    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetShaderInfoLog(v_shader, 512, NULL, infoLog);
        printf("Vertex shader compilation error: %s", infoLog);
    }
    
    const GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader, 1, &fs_code, NULL);
    glCompileShader(f_shader);
    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetShaderInfoLog(f_shader, 512, NULL, infoLog);
        fprintf("Fragment shader compilation error: %s", infoLog);
    }

    program->program = glCreateProgram();
    glAttachShader(program->program, v_shader);
    glAttachShader(program->program, f_shader);
    glLinkProgram(program->program);
    glGetProgramiv(program->program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program->program, 512, NULL, infoLog);
        fprintf("Failed to link shaders into program: %s", infoLog);
    }
    
    return (CENGINE_GPU_Program){.idx = counter};
}

void CENGINE_GPU_DestroyProgram(CENGINE_GPU_Program *handle) 
{
    GLProgram *program = &s_ctx.programs[handle->idx];
    glDeleteProgram(program->program);
    handle->idx = CENGINE_GPU_NULL_INDEX;
}

CENGINE_GPU_Texture CENGINE_GPU_CreateTexture(const unsigned char *pixels, size_t width, size_t height, CENGINE_GPU_TextureFormat format, CENGINE_GPU_TextureInternalFormat internal_format, size_t mip_map_count) 
{   
    static uint16_t counter = -1;
    counter++;
    GLTexture* texture = &s_ctx.textures[counter];
    glGenTextures(1, &texture->tex);
    glBindTexture(GL_TEXTURE_2D, texture->tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, CENGINE_GPU_RENDERER_TextureFormat(format), width, height, 0, CENGINE_GPU_RENDERER_TextureInternalFormat(internal_format), GL_UNSIGNED_BYTE, (void*)pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return (CENGINE_GPU_Texture){.idx = counter};
}

void CENGINE_GPU_DestroyTexture(CENGINE_GPU_Texture *handle) 
{
    GLTexture *texture = &s_ctx.textures[handle->idx];
    glDeleteTextures(1, &texture->tex);
    handle->idx = CENGINE_GPU_NULL_INDEX;
}

void CENGINE_GPU_DrawIndexed(size_t count, CENGINE_GPU_VertexInput vertex_input, CENGINE_GPU_Program program, CENGINE_GPU_Texture tex) 
{
    GLProgram *p = &s_ctx.programs[program.idx];
    GLVertexInput *input = &s_ctx.vertex_inputs[vertex_input.idx];
    GLTexture *texture = &s_ctx.textures[tex.idx];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.12, 1.0);
    
    glBindVertexArray(input->vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->tex);
    glUseProgram(p->program);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);

}
