#ifndef OPENGL_INTERFACE_H
#define OPENGL_INTERFACE_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum OGL_TYPE {
    //can't be eveuated properly
    OGL_TYPE_UNKNOWN = 0,
    OGL_TYPE_MESH ,
    OGL_TYPE_VERTEX,
    OGL_TYPE_PIXEL,
} OGL_TYPE;

// Function to initialize OpenGL
int OpenGL_Init(const char * name, int width, int height, void** vg);

// Function to initialize OpenGL Resources
int OpenGL_InitResource(void* ctx, void * resource);

void OpenGL_SetResource(void* ctx, void * resource);

void OpenGL_UninitResource(void* ctx, void *resource);


#ifdef __cplusplus
}
#endif

#endif // OPENGL_INTERFACE_H
