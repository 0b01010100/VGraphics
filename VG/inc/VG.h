#ifndef VG_H
#define VG_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//Represents the Graphics Engine
typedef struct VG VG;

// Function Error Returns
typedef enum VG_RESULT {
    VG_SUCCESS = 0ULL,              // Operation was successful
    VG_ERROR_INVALID_PARAMETER,     // One or more parameters are invalid
    VG_ERROR_OUT_OF_MEMORY,         // Memory allocation failed
    VG_ERROR_UNKNOWN                // An unknown error occurred
} VG_RESULT;

// Resource Types
typedef enum VG_TYPE {
    VG_TYPE_UNKNOWN = 0,                      // Unspecified or unknown resource type
    VG_TYPE_MESH,                             // 3D model or mesh data
    VG_TYPE_VERTEX,                           // Vertex data (e.g., vertex buffers)
    VG_TYPE_PIXEL,                            // Pixel data (e.g., textures)
    VG_TYPE_GEO,                              // Geometry shader (for geometry processing)
    VG_TYPE_COMP,                             // Compute shader (for general-purpose computing on the GPU)
    VG_TYPE_SHADER_PROGRAM,                   // Complete shader program consisting of multiple shaders (e.g., vertex, fragment, etc.)
    VG_TYPE_UNIFORM_BUFFER,                   // Uniform buffer object (for passing uniform data to shaders)
    VG_TYPE_SAMPLER,                          // Sampler object (for controlling how textures are sampled in shaders)
    VG_TYPE_PIPELINE,                         // Rendering pipeline object (encapsulates shader stages and configurations)
    VG_TYPE_RENDERBUFFER,                     // Renderbuffer object (used for offscreen rendering, typically for depth/stencil buffers)
    VG_TYPE_MATERIAL,                         // Material properties (may include references to textures, shaders, and other resources)
    VG_TYPE_TEXTURE,                          // Texture object (2D, 3D, or cubemap textures)
    VG_TYPE_TESSELLATION_CONTROL_SHADER,      // Tessellation control shader (for controlling tessellation levels)
    VG_TYPE_TESSELLATION_EVALUATION_SHADER    // Tessellation evaluation shader (for evaluating tessellated vertices)
} VG_TYPE;

//Resource Descriptors 
typedef struct VG_Resource_Desc {
    VG_TYPE type;               // Resource type
    size_t size;                // Size of the data (in bytes)
    void *data;           // Pointer to raw data (e.g., vertex data, pixel data)
    const char *name;           // Name or identifier of the resource
    const char *file_or_string; // Path to file or shader code as a string (for shaders)
    unsigned int width;         // Width for textures/renderbuffers
    unsigned int height;        // Height for textures/renderbuffers
    unsigned int depth;         // Depth for 3D textures or other 3D resources
    unsigned int mipLevels;     // Mipmap levels for textures
    unsigned int arrayLayers;   // Number of layers for texture arrays
    unsigned int format;        // Format of the data (e.g., pixel format, vertex format)
} VG_Resource_Desc;

#if defined(DEBUG)
/* 
    Represents a Graphics Resource
    @warning For Debug Mode only
*/
typedef struct VG_Resource
{
    VG_Resource_Desc desc;
    unsigned int data[4];
} VG_Resource;
#else
/* 
    Represents a Graphics Resource
*/
typedef unsigned int VG_Resource;
#endif

//Window Descriptor
typedef struct VG_Win {
    int width; // Window width
    int height; // Window Height
    const char *name; // Window Name
} VG_Win;

//Init the Graphics engine
VG *VG_ctor(VG_Win config);

//Create Graphics Resources
VG_Resource* VG_LoadResource(VG *this_, VG_Resource_Desc* desc, VG_RESULT* err);

//Set Graphics Resources to be used
void VG_SetResources(VG *vg, unsigned int count, ...);

//Destroyes Graphics Resources
void VG_UnLoadResources(VG *vg, unsigned int count, ...);

//Render on to a frame buffer
void VG_Render(VG *vg);

//Clears a frame buffer
void VG_Clear_Buffer(VG *vg);

//Frees Graphics Engine
void VG_dtor(VG *vg);

#ifdef __cplusplus
}
#endif

#endif // VG_H
