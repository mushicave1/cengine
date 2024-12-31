#include <cengine/cengine.h>
#include <core/core.h>

#include "gpu.h"

typedef struct {} VKVertexBuffer;
typedef struct {} VKIndexBuffer;
typedef struct {} VKProgram;
typedef struct {} VKTexture;
typedef struct {} VKVertexInput;

typedef struct {
    VKVertexBuffer vertex_buffers[CENGINE_GPU_MAX_VERTEX_BUFFERS];
    VKIndexBuffer index_buffers[CENGINE_GPU_MAX_INDEX_BUFFERS];
    VKVertexInput vertex_inputs[CENGINE_GPU_MAX_VERTEX_INPUTS];
    VKProgram programs[CENGINE_GPU_MAX_PROGRAMS];
    VKTexture textures[CENGINE_GPU_MAX_TEXTURES];
} CENGINE_GPU_Context;

static CENGINE_GPU_Context s_ctx;

static inline int CENGINE_GPU_RENDERER_TextureFormat(CENGINE_GPU_TextureFormat format) 
{
    /** Unimplemented */
}

static inline int CENGINE_GPU_RENDERER_TextureInternalFormat(CENGINE_GPU_TextureInternalFormat internal_format) 
{
    /** Unimplemented */
}


CENGINE_GPU_VertexBuffer CENGINE_GPU_CreateVertexBuffer(void *data, size_t byte_size) 
{ 
    /** Unimplemented */
}

void CENGINE_GPU_DestroyVertexBuffer(CENGINE_GPU_VertexBuffer *handle) 
{
    /** Unimplemented */
}

CENGINE_GPU_IndexBuffer CENGINE_GPU_CreateIndexBuffer(void *data, size_t byte_size) 
{
    /** Unimplemented */
}

void CENGINE_GPU_DestroyIndexBuffer(CENGINE_GPU_IndexBuffer *handle) 
{
    /** Unimplemented */
}

CENGINE_GPU_VertexInput CENGINE_GPU_CreateVertexInput(CENGINE_GPU_VertexBuffer vertex_buffer, CENGINE_GPU_BufferView buffer_view, CENGINE_GPU_IndexBuffer index_buffer) 
{
    /** Unimplemented */
}

void CENGINE_GPU_DestroyVertexInput(CENGINE_GPU_VertexInput *handle) 
{
    /** Unimplemented */
}


CENGINE_GPU_Program CENGINE_GPU_CreateProgram(const char *vs_code, const char *fs_code) 
{
    /** Unimplemented */
}

void CENGINE_GPU_DestroyProgram(CENGINE_GPU_Program *handle) 
{
    /** Unimplemented */
}

CENGINE_GPU_Texture CENGINE_GPU_CreateTexture(const unsigned char *pixels, size_t width, size_t height, CENGINE_GPU_TextureFormat format, CENGINE_GPU_TextureInternalFormat internal_format, size_t mip_map_count) 
{   
    /** Unimplemented */
}

void CENGINE_GPU_DestroyTexture(CENGINE_GPU_Texture *handle) 
{
    /** Unimplemented */
}

void CENGINE_GPU_DrawIndexed(size_t count, CENGINE_GPU_VertexInput vertex_input, CENGINE_GPU_Program program) 
{
    /** Unimplemented */
}
