#include <VG/inc/VG.h>
#include <stdlib.h>
// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";
int main()
{
    VG_Win l = {0};
    l.height = 300;
    l.width = 300;
    l.name = "Window";
    VG* vg = VG_Create(l);

    // SHADERS

    VG_Resource_Desc vs = {};
    vs.file_or_string = vertexShaderSource;
    vs.type = VG_TYPE_VERTEX;
    VG_Resource* r = VG_LoadResource(vg, &vs, 0);


    VG_Resource_Desc fs = {};
    fs.file_or_string = fragmentShaderSource;
    fs.type = VG_TYPE_PIXEL;
    VG_Resource* r1 = VG_LoadResource(vg, &fs, 0);


    // SHADER PROGRAM

    VG_Resource_Desc sp = {};
    sp.type = VG_TYPE_SHADER_PROGRAM;
    sp.name = "BasicShaderProgram";
    sp.size = 2;
    VG_Resource* shaders[2];
    shaders[0] = r;
    shaders[1] = r1;
    sp.data = shaders;
    
    VG_Resource* ShaderProgram = VG_LoadResource(vg, &sp, 0);


    
    // USE RESOURCES
    VG_SetResources(vg, 1, ShaderProgram);

    while (true)
    {
        VG_FrameStartA(vg, VG_COLOR_BLACK);


        //TODO


        VG_FrameEnd(vg);
    }
    
}