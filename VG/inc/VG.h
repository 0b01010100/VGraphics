#ifndef VG_H
#define VG_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct VG VG;

typedef enum VG_RESULT {
    VG_SUCCESS = 0,
    VG_ERROR_INVALID_PARAMETER,
    VG_ERROR_OUT_OF_MEMORY,
    VG_ERROR_UNKNOWN
} VG_RESULT;

typedef enum VG_TYPE {
    VG_TYPE_UNKNOWN = 0,
    VG_TYPE_MESH,
    VG_TYPE_VERTEX,
    VG_TYPE_PIXEL

} VG_TYPE;

typedef  struct VG_Resource
{
    unsigned int api_specific_data[4];
    const char * file_or_string;
    VG_TYPE type;
}VG_Resource;
// typedef enum VG_TYPE {
//     VG_TYPE_UNKNOWN = 0,
//     VG_TYPE_MESH,
//     VG_TYPE_SHADER_PROGRAM,
//     VG_TYPE_TEXTURE,
//     VG_TYPE_MATERIAL,
//     VG_TYPE_FRAMEBUFFER,
//     VG_TYPE_RENDERBUFFER,
//     VG_TYPE_UNIFORM_BUFFER,
//     VG_TYPE_SAMPLER,
//     VG_TYPE_PIPELINE,
//     VG_TYPE_COMPUTE_SHADER,
//     VG_TYPE_GEOMETRY_SHADER,
//     VG_TYPE_TESSELLATION_CONTROL_SHADER,
//     VG_TYPE_TESSELLATION_EVALUATION_SHADER
// } VG_TYPE;
// typedef struct VG_Shader
// {
//     const char * src;
//     unsigned int shader_data[2];
// }VG_Shader;
// typedef struct VG_ShaderProgram {

//     VG_Shader* shaders;
//     // const char *vs; // Vertex Shader
//     // const char *fs; // Fragment Shader
//     // const char *hs; // Hull Shader
//     // const char *gs; // Geometry Shader
//     // const char *cs; // Compute Shader
//     // const char *tcs; // Tessellation Control Shader
//     // const char *tes; // Tessellation Evaluation Shader
//     //unsigned int shader_data[2 * 7]; // Shader-specific data
// } VG_ShaderProgram;

typedef struct VG_Win {
    int width;
    int height;
    const char *name;
} VG_Win;

VG *VG_ctor(VG_Win config);

void VG_Window_Loop(VG *vg);

#define VG_Resource_Param(res) (&res)
VG_RESULT VG_ctor_Resources(VG *vg, unsigned char count, ...);

void VG_Set_Resources(VG *vg, unsigned char count, ...);

void VG_dtor_Resources(VG *vg, unsigned char count, ...);

void VG_Render(VG *vg);

void VG_Clear_Buffer(VG *vg);

void VG_dtor(VG *vg);

#ifdef __cplusplus
}
#endif

#endif // VG_H
