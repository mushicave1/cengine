#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "image_loader.h"


void CENGINE_IMAGE_LOADER_Load(const char *image_path, size_t *pixel_width, size_t *pixel_height, size_t *channels, unsigned char **pixels) 
{
    *pixels = stbi_load(image_path, pixel_width, pixel_height, channels, 0);
}