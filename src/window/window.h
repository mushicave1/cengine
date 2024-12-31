#ifndef CENGINE_WINDOW_H
#define CENGINE_WINDOW_H

#define GLFW_INCLUDE_VULKAN

#include <core/core.h>
#include <GLFW/glfw3.h>

typedef struct CENGINE_Window *CENGINE_Window;

void CENGINE_WINDOW_Init(size_t pixel_width, size_t pixel_height, const char *name, CENGINE_Window *window);

bool CENGINE_WINDOW_IsRunning(CENGINE_Window window);

void CENGINE_WINDOW_ListenToUserInput(CENGINE_Window window);

void CENGINE_WINDOW_FinishFrame(CENGINE_Window window);

void CENGINE_WINDOW_Destroy(CENGINE_Window window);

#endif