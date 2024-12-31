#ifndef CENGINE_SRC_IMAGE_LOADAER_H
#define CENGINE_SRC_IMAGE_LOADAER_H

#include <stdio.h>

typedef struct {

} CENGINE_ImageLoader;

void CENGINE_IMAGE_LOADER_Load(const char *image_path, size_t *pixel_width, size_t *pixel_height, size_t *channels, unsigned char **pixels);

#endif /** CENGINE_SRC_IMAGE_LOADAER_H */