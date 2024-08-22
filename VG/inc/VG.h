#ifndef VG_H
#define VG_H

#include <stdint.h>
#include <string.h>

#define VG_NONE (0)
//Represents A bool
typedef int VG_BOOL;
#if __cplusplus
//Aggregate initialization 
#define VG_default {}
//Represents A nullptr
#define VG_nullptr nullptr
//VG_BOOL true
#define VG_TRUE true
//VG_BOOL false
#define VG_FALSE false
#else//C
//Aggregate initialization
#define VG_default {0}
//Represents A nullptr
#define VG_nullptr NULL
//VG_BOOL true
#define VG_TRUE 0x1
//VG_BOOL false
#define VG_FALSE 0x0
#endif

//Represents Black with full opacity
const float VG_COLOR_BLACK[4] = {0.0F, 0.0F, 0.0F, 1.0F};

//Represents White with full opacity
const float VG_COLOR_WHITE[4] = {1.0F, 1.0F, 1.0F, 1.0F};

// Function Error Returns
typedef enum VG_RESULT/* : int*/
{
    VG_SUCCESS                 = 0x0,//0: Operation was successful
    VG_ERROR_NULL              = -1,//-1: NULL
    VG_ERROR_INVALID_PARAMETER = -2,//-2: One or more parameters are invalid
    VG_ERROR_OUT_OF_MEMORY     = -3,//-3: Memory allocation failed
    VG_ERROR_CANT_OPEN_FILE    = -4,//-4: Cannot open file for reading
    VG_ERROR_FILE_READ         = -5,//-5: Error occurred while reading the file
    VG_ERROR_SHADER_COMPILE    = -6,//-6: Shader compilation failed
    VG_ERROR_PARSING           = -6,//-6 Faild to parse source
    VG_ERROR_SHADER_LINK       = -7,//-7 Faild to link shader
    VG_ERROR_CREATION          = -8,//-8: Resource creation faild
    VG_ERROR_UNKNOWN           = -9 //-9: An unknown error occurred
} VG_RESULT;

// Resource Types
typedef enum VG_TYPE {
    VG_TYPE_UNKNOWN = 0,                      //Unspecified or unknown resource type
    VG_TYPE_MESH,                             //3D model or mesh data
    VG_TYPE_VERTEX,                           //Vertex data (e.g., vertex buffers)
    VG_TYPE_PIXEL,                            //Pixel data (e.g., textures)
    VG_TYPE_GEO,                              //Geometry shader (for geometry processing)
    VG_TYPE_COMP,                             //Compute shader (for general-purpose computing on the GPU)
    VG_TYPE_TESSELLATION_CONTROL_SHADER,      //Tessellation control shader (for controlling tessellation levels)
    VG_TYPE_TESSELLATION_EVALUATION_SHADER,   //Tessellation evaluation shader (for evaluating tessellated vertices)
    VG_TYPE_SHADER_PROGRAM,                   //Complete shader program consisting of multiple shaders (e.g., vertex, fragment, etc.)
    VG_TYPE_UNIFORM_BUFFER,                   //Uniform buffer object (for passing uniform data to shaders)
    VG_TYPE_SAMPLER,                          //Sampler object (for controlling how textures are sampled in shaders)
    VG_TYPE_PIPELINE,                         //Rendering pipeline object (encapsulates shader stages and configurations)
    VG_TYPE_RENDERBUFFER,                     //Renderbuffer object (used for offscreen rendering, typically for depth/stencil buffers)
    VG_TYPE_MATERIAL,                         //Material properties (may include references to textures, shaders, and other resources)
    VG_TYPE_TEX1D,                            //Texture object (1D)
    VG_TYPE_TEX2D,                            //Texture object (2D)
    VG_TYPE_TEX3D,                            //Texture object (3D)
} VG_TYPE;

/*
    Formats of resource soruce
*/
typedef enum VG_SORUCE_FORMAT
{
    // Flag for if the source is a file
    VG_SORUCE_FORMAT_FILE = 0x0,
    // Flag for if the source is a string
    VG_SORUCE_FORMAT_STRING
}VG_SORUCE_FORMAT;

/*
    Data Formats
*/
typedef enum VG_FORMAT
{
    VG_RGBA8,   //8 bits per channel for Red, Green, Blue, and Alpha
    VG_RGB8,    //8 bits per channel for Red, Green, and Blue (no Alpha)
    VG_R8,      //8 bits for a single Red channel.
}VG_FORMAT;

//Resource Descriptors 
typedef struct VG_Resource_Desc {
    VG_TYPE type;               //Resource type
    size_t size;                //Size of the data (in bytes)
    void *data;                 //Pointer to raw data (e.g., vertex data, pixel data)
    const char *str_id;         //Name or identifier of the resource
    const char *file_or_string; //Path to file or shader code as a string (for shaders)
    unsigned int width;         //Width for textures/renderbuffers
    unsigned int height;        //Height for textures/renderbuffers
    unsigned int depth;         //Depth for 3D textures or other 3D resources
    unsigned int mipLevels;     //Mipmap levels for textures
    unsigned int arrayLayers;   //Number of layers for texture arrays
    unsigned int format;        //Format of the data (e.g., pixel format, vertex format)
} VG_Resource_Desc;

#if defined(DEBUG) || defined(_DEBUG)
/* 
    Represents a Graphics Resource
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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A C style constructor for Pixel Shader descriptor.
 * 
 * This function initializes a `VG_Resource_Desc` structure for a pixel shader.
 * @param type The type of shader to use
 * @param file_or_string A string containing either the file path or the shader code.
 * @param is_file non-zero indicates that `file_or_string` is a file path, and zero indicates it's a string of shader code.
 * @param str_id A string identifier for the shader resource.
 * 
 * @return VG_Resource_Desc A descriptor for the pixel shader resource.
*/
static inline VG_Resource_Desc VG_SHADER_DESC(VG_TYPE type, const char * file_or_string, VG_SORUCE_FORMAT src_format, const char * str_id){ 
    VG_Resource_Desc desc = VG_default;
    desc.type = type;
    desc.file_or_string = file_or_string;
    desc.str_id = str_id;
    desc.format = src_format;
    return desc;
}

/**
 * @brief A C style constructor for Vertex Shader descriptor.
 * 
 * This function initializes a `VG_Resource_Desc` structure for a vertex shader.
 * 
 * @param file_or_string A string containing either the file path or the shader code.
 * @param is_file A boolean-like integer where non-zero indicates that `file_or_string` is a file path, and zero indicates it's a string of shader code.
 * @param str_id A string identifier for the shader resource.
 * 
 * @return VG_Resource_Desc A descriptor for the vertex shader resource.
*/
#define VG_VSHADER_DESC(file_or_string, src_format, str_id)\
    VG_SHADER_DESC(VG_TYPE_VERTEX, file_or_string, src_format, str_id)

/**
 * @brief A C style constructor for Pixel Shader descriptor.
 * 
 * This function initializes a `VG_Resource_Desc` structure for a pixel shader.
 * 
 * @param file_or_string A string containing either the file path or the shader code.
 * @param is_file non-zero indicates that `file_or_string` is a file path, and zero indicates it's a string of shader code.
 * @param str_id A string identifier for the shader resource.
 * 
 * @return VG_Resource_Desc A descriptor for the pixel shader resource.
*/
#define VG_PSHADER_DESC(file_or_string, src_format, str_id)\
    VG_SHADER_DESC(VG_TYPE_PIXEL, file_or_string, src_format, str_id)
/**
 * @brief A C style constructor for Pixel Shader descriptor.
 * 
 * This function initializes a `VG_Resource_Desc` structure for a pixel shader.
 * 
 * @param file_or_string A string containing either the file path or the shader code.
 * @param is_file A boolean-like integer where non-zero indicates that `file_or_string` is a file path, and zero indicates it's a string of shader code.
 * @param str_id A string identifier for the shader resource.
 * 
 * @return VG_Resource_Desc A descriptor for the (fragment/pixel) shader resource.
*/
#define VG_FSHADER_DESC VG_PSHADER_DESC

/**
 * @brief A C style constructor for Shader Program descriptor.
 * 
 * This function initializes a `VG_Resource_Desc` structure for a shader program.
 * 
 * @param str_id A string identifier for the shader program resource.
 * @param arr_shaders A pointer to an array of `VG_Resource` pointers representing the shaders used in the program.
 * @param el_count The number of shaders in the `arr_shaders` array.
 * 
 * @return VG_Resource_Desc A descriptor for the shader program resource.
*/
static inline VG_Resource_Desc VG_SHADER_PROGRAM_DESC(const char * str_id, VG_Resource** arr_shaders, size_t el_count)
{
    VG_Resource_Desc desc = VG_default;
    desc.type = VG_TYPE_SHADER_PROGRAM;
    desc.str_id = str_id;
    desc.size = el_count;
    desc.data = arr_shaders;
    return desc;
}


// TODO ADD SAMPLERS


//
/**
 * @brief A generic C style constructor for texture descriptor.
 * 
 * 
 * @param file_or_string A string containing either the file path or the texture data.
 * @param type Texture type
 * @param is_file Non-zero indicates that `file_or_string` is a file path, and zero indicates it's a string of texture data.
 * @param width The width of the texture.   (For 1D/2D/3D Textures)
 * @param height The height of the texture. (For 2D/3D Textures)
 * @param depth The depth of the texture.   (For 3D Textures)
 * @param mipLevels The number of mipmap levels.
 * @param arrayLayers The number of layers in the texture array.
 * @param format The format of the texture data.
 * @param str_id A string identifier for the texture resource.
*/
static inline VG_Resource_Desc VG_TEX_DESC
(
    VG_TYPE type,
    const char* file_or_string, 
    VG_BOOL is_file, 
    unsigned int width, 
    unsigned int height, 
    unsigned int depth, 
    unsigned int mipLevels, 
    unsigned int arrayLayers, 
    unsigned int format, 
    const char * str_id
) 
{
    VG_Resource_Desc desc;
    desc.type = type;
    desc.size = VG_NONE;
    desc.file_or_string = file_or_string;
    desc.width = width;
    desc.height = height;
    desc.depth = depth;
    desc.mipLevels = mipLevels;
    desc.arrayLayers = arrayLayers;
    desc.format = format;
    desc.data = VG_nullptr;
    desc.str_id = str_id;
    return desc;
}

/**
 * @brief A C style constructor for 3D Texture descriptor.
 * 
 * This macro initializes a `VG_Resource_Desc` structure for a 3D texture.
 * 
 * @param file_or_string A string containing either the file path or the texture data.
 * @param is_file A boolean-like integer where non-zero indicates that `file_or_string` is a file path, and zero indicates it's a string of texture data.
 * @param width The width of the 3D texture.
 * @param height The height of the 3D texture.
 * @param depth The depth of the 3D texture.
 * @param mipLevels The number of mipmap levels.
 * @param arrayLayers The number of layers in the texture array.
 * @param format The format of the texture data.
 * @param str_id A string identifier for the texture resource.
*/
#define VG_TEX3D_DESC(file_or_string, is_file, width, height, depth, mipLevels, arrayLayers, format, str_id)\
    VG_TEX_DESC(VG_TYPE_TEX3D, file_or_string, is_file, width, height, depth, mipLevels, arrayLayers, format, str_id)

/**
 * @brief A C style constructor for 2D Texture descriptor.
 * 
 * This macro initializes a `VG_Resource_Desc` structure for a 2D texture.
 * 
 * @param file_or_string A string containing either the file path or the texture data.
 * @param is_file Non-zero indicates that `file_or_string` is a file path, and zero indicates it's a string of texture data.
 * @param width The width of the 2D texture.
 * @param height The height of the 2D texture.
 * @param mipLevels The number of mipmap levels.
 * @param arrayLayers The number of layers in the texture array.
 * @param format The format of the texture data.
 * @param str_id A string identifier for the texture resource.
 */
#define VG_TEX2D_DESC(file_or_string, is_file, width, height, mipLevels, arrayLayers, format, str_id)\
    VG_TEX_DESC(VG_TYPE_TEX2D, file_or_string, is_file, width, height, VG_NONE, mipLevels, arrayLayers, format, str_id)

/**
 * @brief A C style constructor for 1D Texture descriptor.
 * 
 * This macro initializes a `VG_Resource_Desc` structure for a 1D texture.
 * 
 * @param file_or_string A string containing either the file path or the texture data.
 * @param is_file Non-zero indicates that `file_or_string` is a file path, and zero indicates it's a string of texture data.
 * @param width The width of the 1D texture.
 * @param mipLevels The number of mipmap levels.
 * @param arrayLayers The number of layers in the texture array.
 * @param format The format of the texture data.
 * @param str_id A string identifier for the texture resource.
 */
#define VG_TEX1D_DESC(file_or_string, is_file, width, mipLevels, arrayLayers, format, str_id)\
    VG_TEX_DESC(VG_TYPE_TEX1D, file_or_string, is_file, width, VG_NONE, VG_NONE, mipLevels, arrayLayers, format, str_id)




//Represents the Graphics Engine
typedef struct VG VG;

//Init the Graphics engine
VG *VG_Create(const char *name, int width, int height);

//Create Graphics Resources
VG_Resource* VG_LoadResource(VG *this_, VG_Resource_Desc* desc, VG_RESULT* err);

/* 
    Set Graphics Resources to be used
    @warning Do not use &. For Ex VG_SetResources(vg, 1, &Shaderprogram)
*/
void VG_SetResources(VG *vg, unsigned int count, ...);

/* 
    Destroyes Graphics Resources
    @warning Do not use &. For Ex VG_UnLoadResources(vg, 1, &Shaderprogram)
*/
void VG_UnLoadResources(VG *vg, unsigned int count, ...);

//Render on to a frame buffer using RGBA Arrary
#define VG_FrameStartA(vg, rgba) VG_FrameStart(vg, rgba[0], rgba[1], rgba[2], rgba[3])

//Render on to a frame buffer using RGBA
void VG_FrameStart(VG *vg, float r, float g, float b, float a);

//Clears a frame buffer
void VG_FrameEnd(VG *vg);

//Frees Graphics Engine
void VG_Destroy(VG *vg);

#ifdef __cplusplus
}
#endif

#endif // VG_H
