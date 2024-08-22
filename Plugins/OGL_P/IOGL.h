#ifndef OPENGL_INTERFACE_H
#define OPENGL_INTERFACE_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Function Error Returns
typedef enum OGL_RESULT/* : int*/
{
    OGL_SUCCESS                 = 0x0,//0: Operation was successful
    OGL_ERROR_NULL              = -1,//-1: NULL
    OGL_ERROR_INVALID_PARAMETER = -2,//-2: One or more parameters are invalid
    OGL_ERROR_OUT_OF_MEMORY     = -3,//-3: Memory allocation failed
    OGL_ERROR_CANT_OPEN_FILE    = -4,//-4: Cannot open file for reading
    OGL_ERROR_FILE_READ         = -5,//-5: Error occurred while reading the file
    OGL_ERROR_SHADER_COMPILE    = -6,//-6: Shader compilation failed
    OGL_ERROR_PARSING           = -6,//-6 Faild to parse source
    OGL_ERROR_SHADER_LINK       = -7,//-7 Faild to link shader
    OGL_ERROR_CREATION          = -8,//-8: Resource creation faild
    OGL_ERROR_UNKNOWN           = -9 //-9: An unknown error occurred
} OGL_RESULT;

// Resource Types
typedef enum OGL_TYPE {
    OGL_TYPE_UNKNOWN = 0,                      //Unspecified or unknown resource type
    OGL_TYPE_MESH,                             //3D model or mesh data
    OGL_TYPE_VERTEX,                           //Vertex data (e.g., vertex buffers)
    OGL_TYPE_PIXEL,                            //Pixel data (e.g., textures)
    OGL_TYPE_GEO,                              //Geometry shader (for geometry processing)
    OGL_TYPE_COMP,                             //Compute shader (for general-purpose computing on the GPU)
    OGL_TYPE_TESSELLATION_CONTROL_SHADER,      //Tessellation control shader (for controlling tessellation levels)
    OGL_TYPE_TESSELLATION_EVALUATION_SHADER,   //Tessellation evaluation shader (for evaluating tessellated vertices)
    OGL_TYPE_SHADER_PROGRAM,                   //Complete shader program consisting of multiple shaders (e.g., vertex, fragment, etc.)
    OGL_TYPE_UNIFORM_BUFFER,                   //Uniform buffer object (for passing uniform data to shaders)
    OGL_TYPE_SAMPLER,                          //Sampler object (for controlling how textures are sampled in shaders)
    OGL_TYPE_PIPELINE,                         //Rendering pipeline object (encapsulates shader stages and configurations)
    OGL_TYPE_RENDERBUFFER,                     //Renderbuffer object (used for offscreen rendering, typically for depth/stencil buffers)
    OGL_TYPE_MATERIAL,                         //Material properties (may include references to textures, shaders, and other resources)
    OGL_TYPE_TEX1D,                            //Texture object (1D)
    OGL_TYPE_TEX2D,                            //Texture object (2D)
    OGL_TYPE_TEX3D,                            //Texture object (3D)
} OGL_TYPE;

/*
    Formats of resource soruce
*/
typedef enum OGL_SORUCE_FORMAT
{   
    // Flag for if the source is a file
    OGL_SORUCE_FORMAT_FILE   = 0x0, 
    // Flag for if the source is a string
    OGL_SORUCE_FORMAT_STRING = 0x1  
}OGL_SORUCE_FORMAT;

/*
    Data Formats
*/
typedef enum OGL_FORMAT
{
    OGL_RGBA8,   //8 bits per channel for Red, Green, Blue, and Alpha
    OGL_RGB8,    //8 bits per channel for Red, Green, and Blue (no Alpha)
    OGL_R8,      //8 bits for a single Red channel.
}OGL_FORMAT;

//OGL resource desc
typedef struct OGL_Resource_Desc {
    OGL_TYPE type;              // Resource type
    size_t size;                // Size of the data (in bytes)
    void *data;                 // Pointer to raw data (e.g., vertex data, pixel data)
    const char *name;           // Name or identifier of the resource
    const char *file_or_string; // Path to file or shader code as a string (for shaders)
    unsigned int width;         // Width for textures/renderbuffers
    unsigned int height;        // Height for textures/renderbuffers
    unsigned int depth;         // Depth for 3D textures or other 3D resources
    unsigned int mipLevels;     // Mipmap levels for textures
    unsigned int arrayLayers;   // Number of layers for texture arrays
    unsigned int format;        // Format of the data (e.g., pixel format, vertex format)
} OGL_Resource_Desc;

typedef struct OGL_Resource OGL_Resource;

// Function to initialize OpenGL
int OGL_Init(const char * name, int width, int height, void** vg);

// Clears the buffer with the given values
void OGL_StartFrame(void* ctx, float color[4]);

// Renders a frame
void OGL_EndFrame(void* ctx);

// Function to initialize OpenGL Resources
OGL_RESULT OGL_LoadResource(void* ctx, void * desc, void** out);

//Set Resources to be using during rendering
void OGL_SetResource(void* ctx, void * resource);

//Destroys resources
void OGL_UninitResource(void* ctx, void * rs);


#ifdef __cplusplus
}
#endif

#endif // OPENGL_INTERFACE_H
