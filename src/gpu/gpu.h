#ifndef CENGINE_SRC_GPU_GPU_H
#define CENGINE_SRC_GPU_GPU_H

#include <stdint.h>
#include <core/core.h>

#define CENGINE_GPU_HANDLE(TYPE) \
    typedef struct { \
        uint16_t idx; \
    } TYPE; \

#define CENGINE_GPU_ASSERT_HANDLE_IS_VALID(HANDLE) ((HANDLE).idx != CENGINE_GPU_INVALID_HANDLE)

CENGINE_GPU_HANDLE(CENGINE_GPU_VertexBuffer);
CENGINE_GPU_HANDLE(CENGINE_GPU_IndexBuffer);
CENGINE_GPU_HANDLE(CENGINE_GPU_VertexInput);
CENGINE_GPU_HANDLE(CENGINE_GPU_Program);
CENGINE_GPU_HANDLE(CENGINE_GPU_Texture);

#define CENGINE_GPU_NULL_INDEX (-1)
#define CENGINE_GPU_INVALID_HANDLE (-1)

#define CENGINE_GPU_MAX_VERTEX_BUFFERS 100
#define CENGINE_GPU_MAX_INDEX_BUFFERS 100
#define CENGINE_GPU_MAX_VERTEX_INPUTS 100
#define CENGINE_GPU_MAX_PROGRAMS 100
#define CENGINE_GPU_MAX_TEXTURES 100


typedef enum {
    CENGINE_GPU_BACKEND_GL,
    CENGINE_GPU_BACKEND_VK
} CENGINE_GPU_Backend;

typedef enum {
    CENGINE_GPU_TEXTURE_FORMAT_R,
    CENGINE_GPU_TEXTURE_FORMAT_RG,
    CENGINE_GPU_TEXTURE_FORMAT_RGB,
    CENGINE_GPU_TEXTURE_FORMAT_RGBA
} CENGINE_GPU_TextureFormat;

typedef enum {
    CENGINE_GPU_TEXTURE_INTERNAL_FORMAT_RGB32,
    CENGINE_GPU_TEXTURE_INTERNAL_FORMAT_RGB32F,
    CENGINE_GPU_TEXTURE_INTERNAL_FORMAT_RGBA32,
    CENGINE_GPU_TEXTURE_INTERNAL_FORMAT_RGBA32F
} CENGINE_GPU_TextureInternalFormat;

typedef struct {
    uint32_t stride;
    uint32_t *offsets;
    uint32_t *element_counts;
    uint32_t len;
} CENGINE_GPU_BufferView;


CENGINE_GPU_VertexBuffer CENGINE_GPU_CreateVertexBuffer(void *data, size_t byte_size);
void CENGINE_GPU_DestroyVertexBuffer(CENGINE_GPU_VertexBuffer *handle);

CENGINE_GPU_IndexBuffer CENGINE_GPU_CreateIndexBuffer(void *data, size_t byte_size);
void CENGINE_GPU_DestroyIndexBuffer(CENGINE_GPU_IndexBuffer *handle);

CENGINE_GPU_Program CENGINE_GPU_CreateProgram(const char *vs_code, const char *fs_code);
void CENGINE_GPU_DestroyProgram(CENGINE_GPU_Program *handle);

CENGINE_GPU_VertexInput CENGINE_GPU_CreateVertexInput(CENGINE_GPU_VertexBuffer vertex_buffer, CENGINE_GPU_BufferView buffer_view, CENGINE_GPU_IndexBuffer index_buffer);
void CENGINE_GPU_DestroyVertexInput(CENGINE_GPU_VertexInput *handle);

CENGINE_GPU_Texture CENGINE_GPU_CreateTexture(const unsigned char *pixels, size_t width, size_t height, CENGINE_GPU_TextureFormat format, CENGINE_GPU_TextureInternalFormat internal_format, size_t mip_map_count);
void CENGINE_GPU_DestroyTexture(CENGINE_GPU_Texture *handle);

/*
* Draw Commands
*/
void CENGINE_GPU_DrawIndexed(size_t count, CENGINE_GPU_VertexInput vertex_input, CENGINE_GPU_Program program, CENGINE_GPU_Texture tex);


#endif /** CENGINE_SRC_GPU_GPU_H */